#include "gestion_modeles.h"





Modele* create_modele() {
    Modele* model_ptr = malloc(sizeof(Modele));
    
    model_ptr->taille_x = 2;
    model_ptr->taille_y = 2;
    model_ptr->rayon_poids = 0.10;
    model_ptr->k = 20;
    model_ptr->longueur_ressorts_repos = 1.0;
    model_ptr->nb_poids_fixes = 2;
    
    model_ptr->tableau = init_tableau_exemple(2,2, 1);
    
    model_ptr->taille_tableau_ressorts = 4;
    model_ptr->tableau_ressorts = malloc(model_ptr->taille_tableau_ressorts * sizeof(int*));

    int ressorts_initiaux[4][2] = {
        {0, 1},
        {0, 2},
        {1, 3},
        {2, 3}
    };

    for (int i = 0; i < model_ptr->taille_tableau_ressorts; i++) {
        model_ptr->tableau_ressorts[i] = malloc(2 * sizeof(int));
        model_ptr->tableau_ressorts[i][0] = ressorts_initiaux[i][0];
        model_ptr->tableau_ressorts[i][1] = ressorts_initiaux[i][1];
    }

    model_ptr->liste_index_poids_fixes = malloc(model_ptr->nb_poids_fixes * sizeof(int));
    model_ptr->liste_index_poids_fixes[0] = 2;
    model_ptr->liste_index_poids_fixes[1] = 1;
    
    return model_ptr;
}



void free_modele(Modele* model_ptr) {
    if (model_ptr == NULL) {
        return;
    }

    // Libérer les ressources des tableaux dynamiques
    if (model_ptr->tableau_ressorts != NULL) {
        for (int i = 0; i < model_ptr->taille_tableau_ressorts; i++) {
            free(model_ptr->tableau_ressorts[i]);
        }
        free(model_ptr->tableau_ressorts);
    }

    free(model_ptr->liste_index_poids_fixes);

    // Libérer les instances de Poids
    if (model_ptr->tableau != NULL) {
        free(model_ptr->tableau);
    }

    // Libérer l'instance de Modele
    //free(model_ptr);
}



Modele* load_modele(const char* filename){
   //const char* filename = "modele.json";
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
      printf("Impossible d'ouvrir le fichier JSON.\n");
      exit(1);
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char* json_data = (char*)malloc(file_size + 1);
  if (json_data == NULL) {
      printf("Impossible d'allouer suffisamment de mémoire.\n");
      fclose(file);
      exit(1);
  }

  printf("   nb b = %ld\n",fread(json_data, file_size, 1, file));
  json_data[file_size] = '\0';

  fclose(file);

  cJSON* root = cJSON_Parse(json_data);
  if (root == NULL) {
      printf("Erreur lors de l'analyse du fichier JSON.\n");
      free(json_data);
      exit(1);
  }
  Modele* model_ptr = malloc(sizeof(Modele));


    model_ptr->taille_x = cJSON_GetObjectItem(root, "taille_x")->valueint;
    model_ptr->taille_y = cJSON_GetObjectItem(root, "taille_y")->valueint;
    model_ptr->rayon_poids = (float)cJSON_GetObjectItem(root, "rayon_poids")->valuedouble;
    model_ptr->k = (float)cJSON_GetObjectItem(root, "k")->valuedouble;
    model_ptr->longueur_ressorts_repos = (float)cJSON_GetObjectItem(root, "longueur_ressorts_repos")->valuedouble;
    model_ptr->nb_poids_fixes = cJSON_GetObjectItem(root, "nb_poids_fixes")->valueint;
    model_ptr->taille_tableau_ressorts = cJSON_GetObjectItem(root, "taille_tableau_ressorts")->valueint;

    // Récuperation des listes
    //tableau poids 
    cJSON* tableau_item = cJSON_GetObjectItem(root, "tableau");
    int tableau_size = cJSON_GetArraySize(tableau_item);
    model_ptr->tableau = (Poids*)malloc(sizeof(Poids) * model_ptr->taille_x*model_ptr->taille_y);

    for(int i=0;i < tableau_size;i++){
        cJSON* poids = cJSON_GetArrayItem(tableau_item, i);
        cJSON* pos = cJSON_GetObjectItem(poids, "position");
        cJSON* vi = cJSON_GetObjectItem(poids, "vitesse_instantanee");

        model_ptr->tableau[i].position.x = (float)cJSON_GetObjectItem(pos, "x")->valuedouble;
        model_ptr->tableau[i].position.y = (float)cJSON_GetObjectItem(pos, "y")->valuedouble;
        model_ptr->tableau[i].position.z = (float)cJSON_GetObjectItem(pos, "z")->valuedouble;

        model_ptr->tableau[i].vitesse_instantanee.x = (float)cJSON_GetObjectItem(vi, "x")->valuedouble;
        model_ptr->tableau[i].vitesse_instantanee.y = (float)cJSON_GetObjectItem(vi, "y")->valuedouble;
        model_ptr->tableau[i].vitesse_instantanee.z = (float)cJSON_GetObjectItem(vi, "z")->valuedouble;

        model_ptr->tableau[i].masse = cJSON_GetObjectItem(poids, "masse")->valuedouble;

    }


    //tableau_ressorts
    cJSON* tableau_ressorts_item = cJSON_GetObjectItem(root, "tableau_ressorts");
    int tableau_ressorts_size = cJSON_GetArraySize(tableau_ressorts_item);
    // Allouez de la mémoire pour le tableau de tableaux d'entiers
    model_ptr->tableau_ressorts = (int**)malloc(tableau_ressorts_size * sizeof(int*));

    for (int i = 0; i < tableau_ressorts_size; i++) {
        cJSON* sous_tableau_item = cJSON_GetArrayItem(tableau_ressorts_item, i);
        if (cJSON_IsArray(sous_tableau_item)) {
            int sous_tableau_size = cJSON_GetArraySize(sous_tableau_item);
            // Allouez de la mémoire pour le sous-tableau d'entiers
            int* sous_tableau = (int*)malloc(sous_tableau_size * sizeof(int));
            for (int j = 0; j < sous_tableau_size; j++) {
                cJSON* entier_item = cJSON_GetArrayItem(sous_tableau_item, j);
                sous_tableau[j] = entier_item->valueint;
            }
            model_ptr->tableau_ressorts[i] = sous_tableau;
        }
    }


    //liste poids fixes
    cJSON* tableau_index_item = cJSON_GetObjectItem(root, "liste_index_poids_fixes"); 
    int tableau_index_size = cJSON_GetArraySize(tableau_index_item);
    model_ptr->liste_index_poids_fixes = (int*)malloc(tableau_index_size * sizeof(int));

    for (int i = 0; i < tableau_index_size; i++) {
        model_ptr->liste_index_poids_fixes[i] = cJSON_GetArrayItem(tableau_index_item, i)->valueint;
    }




 cJSON_Delete(root);
  free(json_data);
    return model_ptr;
}

Env load_env(const char* filename){
  //const char* filename = "env.json";
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
      printf("Impossible d'ouvrir le fichier JSON.\n");
      exit(1);
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char* json_data = (char*)malloc(file_size + 1);
  if (json_data == NULL) {
      printf("Impossible d'allouer suffisamment de mémoire.\n");
      fclose(file);
      exit(1);
  }

  printf("   nb b = %ld\n",fread(json_data, file_size, 1, file));
  json_data[file_size] = '\0';

  fclose(file);

  cJSON* root = cJSON_Parse(json_data);
  if (root == NULL) {
      printf("Erreur lors de l'analyse du fichier JSON.\n");
      free(json_data);
      exit(1);
  }

  Env env;

  // Récupérez les valeurs individuelles du fichier JSON
  cJSON* delta_t_item = cJSON_GetObjectItem(root, "delta_t_");
  cJSON* forces_exterieures_item = cJSON_GetObjectItem(root, "forces_exterieures");
  cJSON* viscosite_item = cJSON_GetObjectItem(root, "viscosite");

  // Assignez les valeurs aux membres de la structure Env
  env.delta_t_ = (float)delta_t_item->valuedouble;
  env.forces_exterieures.x = (float)cJSON_GetObjectItem(forces_exterieures_item, "x")->valuedouble;
  env.forces_exterieures.y = (float)cJSON_GetObjectItem(forces_exterieures_item, "y")->valuedouble;
  env.forces_exterieures.z = (float)cJSON_GetObjectItem(forces_exterieures_item, "z")->valuedouble;
  env.viscosite = (float)viscosite_item->valuedouble;

  cJSON_Delete(root);
  free(json_data);
  return env;
}

Env create_env(){
    Vector3 autres_forces = {0.0, 4.5, 0.0};
    Env env;
    env.delta_t_ = 0.1;
    env.forces_exterieures = autres_forces;
    env.viscosite = 0.5;
    return env;
}
