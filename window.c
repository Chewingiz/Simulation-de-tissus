#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <main.h>
#include <lib/cJSON.h>


/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(const char* n_env, const char* n_mod, int gx, int gy);
static void key(int keycode);
static void draw(void);
static void quit(void);
static inline double get_dt(void);


/* largeur et hauteur de la fenêtre */
static int _wW = 1000, _wH = 600;
/*! identifiant du (futur) GLSL program */
static GLuint _pId = 0;
/* identifiant de modèle 3D */
static GLuint _objId = 0;
/* flag mode fil de fer */
static GLboolean _wf_mode = GL_FALSE;
static int _cam = 0; 

Env environnement;
Modele modele;
Complement_Modele comp_modele; // Paramètres calculés une seule fois pour limiter le nombre de calculs
float* flattenedData;
GLuint textureId;

int main(int argc, char ** argv) {
  const char *param1;
  const char *param2;
  int g1=10;
  int g2=10;
  if(!gl4duwCreateWindow(argc, argv, "Simulation", 20, 20, 
			 _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN)){return 1;}

  if (argc < 5) {
    printf("Modele par défaut.\n");
    param1 = "modeles/env.json"; 
    param2 = "modeles/modele.json"; 
  }else{
    param1 = argv[1];
    param2 = argv[2];
    g1 = atoi(argv[3]);
    g2 = atoi(argv[4]);
  }
  init(param1,param2,g1,g2 );
  atexit(quit);
  gl4duwDisplayFunc(draw);
  gl4duwKeyDownFunc(key);
  gl4duwMainLoop();

  return 0;
}

void ap_tableau_poids(float* tableau_1D, Poids* tableau, int size) {
    for (int i = 0; i < size; i++) {
        tableau_1D[i * 3]     = (float)tableau[i].position.x*5;
        tableau_1D[i * 3 + 1] = (float)tableau[i].position.y*5;
        tableau_1D[i * 3 + 2] = (float)tableau[i].position.z*5;
    }
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

void init(const char* n_env, const char* n_mod, int gx, int gy){
  int i;
  _pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
  _objId = gl4dgGenGrid2df (gx, gy);
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

  environnement =  load_env(n_env);//create_env();
  modele = *load_modele(n_mod);//*create_modele();//

  /*Création du Complément modele*/
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
  createVertexTexture(modele.tableau, modele.taille_x, modele.taille_y);
}

void key(int keycode) {
  static int v = 0;
  switch(keycode) {
  case 'w':
    _wf_mode = !_wf_mode;
    glPolygonMode(GL_FRONT_AND_BACK, _wf_mode ? GL_LINE : GL_FILL);
    break;
  case 'v':
    v++;
    _cam = v%2;
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
  if(_cam == 0){
    gl4duLookAtf(15, 15, 20, 0, 0, 0, 0, 1, 0);
  } else{
    gl4duLookAtf(5, 20, 20, 5, 0, 5, 0, 1, 0);
  }
  
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
