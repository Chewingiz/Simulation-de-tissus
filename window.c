#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <main.h>
#include <lib/cJSON.h>

/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(const char* n_env, const char* n_mod);
static void key(int keycode);
static void draw(void);
static void quit(void);
static inline double get_dt(void);

/*!\brief largeur et hauteur de la fenêtre */
static int _wW = 1000, _wH = 600;
/*!\brief identifiant du (futur) GLSL program */
static GLuint _pId = 0;
/*!\brief identifiant de la texture */
//static GLuint _tex = 0;
/*!\brief identifiant de modèle 3D */
static GLuint _objId = 0;
/*!\brief flag mode fil de fer */
static GLboolean _wf_mode = GL_FALSE;


    int taille_tableau_poids = 4;
    Poids tableau_poids[4];
    int tableau_ressorts[4][2] = {{0, 1},{0, 2},{1, 3},{2, 3}};
    int taille_tableau_ressorts = 4;
    float longueur_ressort_repos = 1.0;
    float viscosite = 0.5;
    float rayon = 0.1;
    Vector3 autres_forces = {0.0, 4.5, 0.0};
    //float k = 20.0;

Env environnement;
Modele modele;
Complement_Modele comp_modele; // Paramètres calculés une seule fois pour limiter le nombre de calculs
float* flattenedData;


GLuint textureId;



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


int main(int argc, char ** argv) {
    const char *param1;
    const char *param2;
    int g1=10;
    int g2=10;
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 20, 20, 
			 _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))
    return 1;

  if (argc < 3) {
    printf("Modele par défaut.\n");
    param1 =  "env.json"; 
    param2 = "modele.json"; 
  }else{
    param1 = argv[1];
    param2 = argv[2];
  }

  
  

  init(param1,param );

  atexit(quit);
  gl4duwDisplayFunc(draw);
  gl4duwKeyDownFunc(key);
  gl4duwMainLoop();

  return 0;
}



Vector3** tableau_principal;

void ap_tableau_poids(float* tableau_1D, Poids* tableau, int size) {
    for (int i = 0; i < size; i++) {
        tableau_1D[i * 3]     = (float)tableau[i].position.x*1;
        tableau_1D[i * 3 + 1] = (float)tableau[i].position.y*1;
        tableau_1D[i * 3 + 2] = (float)tableau[i].position.z*1;
    }
}

Vector3** init_table(int rows, int cols) {
    // Allocation dynamique du tableau 2D de Vector3
    Vector3** data = (Vector3**)malloc(rows * sizeof(Vector3*));
    for (int i = 0; i < rows; i++) {
        data[i] = (Vector3*)malloc(cols * sizeof(Vector3));
    }

    // Initialisation du tableau avec des valeurs
  for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
          // Assigner des valeurs aux membres x, y et z
          data[i][j].x = (float)i; // Valeur de x basée sur l'indice de ligne
          data[i][j].y = 0.0f;  // Valeur de y basée sur l'indice de colonne
          data[i][j].z = (float)j;  // Valeur de z constante pour un plan
          if (i>5){data[i][j].y = 1.f;}
      }
  }
	data[9][0].y= 3; 
    return data;
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

  fread(json_data, file_size, 1, file);
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

  fread(json_data, file_size, 1, file);
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


GLuint createVertexTexture(Poids* tableau, int width, int height) {
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Convertir le tableau 2D en un tableau 1D contenant les données consécutives
    flattenedData = (float*)malloc(width*height * 3 * sizeof(float)); //taille x taille y
 ap_tableau_poids(flattenedData, tableau, width * height );
  //printf("fDATA = %f,%f,%f ",flattenedData[4],flattenedData[5],flattenedData[6]);
 // Étape 3 : Configuration des paramètres de texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); // Définit le mode de wrapping sur l'axe des x
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Définit le mode de wrapping sur l'axe des y
    //printf("ok2");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, flattenedData);
    //free(flattenedData);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}


Env create_env(){
    Env env;
    env.delta_t_ = 0.1;
    env.forces_exterieures = autres_forces;
    env.viscosite = 0.5;
    return env;
}

void init(const char* n_env, const char* n_mod){
  int i;
  _pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
  _objId = gl4dgGenGrid2df (100, 100);
  glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  gl4duGenMatrix(GL_FLOAT, "modelMatrix");
  gl4duGenMatrix(GL_FLOAT, "viewMatrix");
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-1, 1, (-1.0f * _wH) / _wW, (1.0f * _wH) / _wW, 2, 100);
  glViewport(0, 0, _wW, _wH);

  //tableau_principal = init_table(10, 10);
  



    environnement =  load_env(n_env);//create_env();
    modele = *load_modele(n_mod);//*create_modele();//

    //printf("k = %f",modele.k);
    //printf("taille_tableau_ressorts %d                      ",modele.taille_tableau_ressorts);

    /*Complement modele*/
    comp_modele.gravite = -modele.tableau[0].masse * g; // La masse est la même pour tout les poids
    //printf("%f",modele.tableau[1].position.x);
    comp_modele.taille_tableau = modele.taille_x * modele.taille_y;
    comp_modele.position_poids_fixes = malloc(modele.nb_poids_fixes*sizeof(Vector3));
    for(i = 0;i<modele.nb_poids_fixes;i++){ // recuperation de toutes les positions des poids fixes
        if(modele.liste_index_poids_fixes[i]<comp_modele.taille_tableau){
            comp_modele.position_poids_fixes[i] = modele.tableau[modele.liste_index_poids_fixes[i]].position;
        }else{
            printf("Erreur de modèle : le poids fixes à la position %d n'existe pas dans le modèle.", i);
        }
    }
    //printf("ok");
  createVertexTexture(modele.tableau, modele.taille_x, modele.taille_y);

  //afficher_positions_tableau(modele.tableau, comp_modele.taille_tableau);
  //printf("fDATA = %f,%f,%f ",flattenedData[4],flattenedData[5],flattenedData[6]);
}

void key(int keycode) {
  switch(keycode) {
  case 'w':
    _wf_mode = !_wf_mode;
    glPolygonMode(GL_FRONT_AND_BACK, _wf_mode ? GL_LINE : GL_FILL);
    break;
  default:
    break;
  }
}

void Maj_tableau(Env env, Modele* mdl, Complement_Modele comp_mdl){
  //printf("\nk = %f\n\n\n",mdl->k);
    calculer_forces_totale_maj_vitesses(mdl->tableau, comp_mdl.taille_tableau, mdl->tableau_ressorts, mdl->taille_tableau_ressorts, mdl->longueur_ressorts_repos, env.viscosite, mdl->rayon_poids, env.forces_exterieures, mdl->k);
    maj_positions(mdl->tableau, comp_mdl.taille_tableau);
    //réinitialisation des positions des points fixes.
    for(int i=0; i<mdl->nb_poids_fixes; i++){
        mdl->tableau[mdl->liste_index_poids_fixes[i]].position = comp_mdl.position_poids_fixes[i];
    }
    //printf("Table = \n ");
    //afficher_positions_tableau(mdl->tableau, comp_modele.taille_tableau);
}
void draw(void) {
  GLfloat Lp[] = { -2.0f, 2.f, 0.0f, 1.0f };
  //float* flattenedData = (float*)malloc(taille_tableau_poids  * 3 * sizeof(float));
  static float dt = 0;
  static float t = 0;
  dt = get_dt();
  static GLfloat angle = 0.0f;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Lp[0] = -4.0f * cos(angle * M_PI / (1.0f * 180.0f));
  GLfloat bleu[] = {0, 0, 1, 1};
  //GLfloat dir[] = {0, 0, 1};
  glUseProgram(_pId);
  gl4duBindMatrix("viewMatrix");
  gl4duLoadIdentityf();
  gl4duLookAtf(15, 15, 20, 0, 0, 0, 0, 1, 0);
  gl4duBindMatrix("modelMatrix");
  gl4duLoadIdentityf();
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);


  //printf("\nk = %f\n\n\n",modele.k);
  if (t>0.1){
    Maj_tableau(environnement, &modele, comp_modele);
    //printf("\nTable2 = \n ");
    //afficher_positions_tableau(modele.tableau, comp_modele.taille_tableau);
    ap_tableau_poids(flattenedData, modele.tableau, comp_modele.taille_tableau);
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, modele.taille_x, modele.taille_y, 0, GL_RGB, GL_FLOAT, flattenedData);
	  t=0;
  }else{
	  t +=dt;
  }



  //printf("Visco %f",environnement.viscosite);
  glUniform1i(glGetUniformLocation(_pId, "textureSampler"),0);
  glUniform1f(glGetUniformLocation(_pId, "phase"), -angle / 180.0f);
  glUniform4fv(glGetUniformLocation(_pId, "Lp"), 1, Lp);
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu);
  gl4dgDraw(_objId);
  glUseProgram(0);
  //free(flattenedData);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void quit(void) {

  free(tableau_principal);
  free(comp_modele.position_poids_fixes);
  free_modele(&modele);
  free(flattenedData);
  gl4duClean(GL4DU_ALL);
}

double get_dt(void) {
  static double t0 = 0.0;
  double t = gl4dGetElapsedTime() / 1000.0, dt = t - t0;
  t0 = t;
  return dt;
}
