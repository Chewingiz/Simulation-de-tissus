#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <main.h>



/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void key(int keycode);
static void draw(void);
static void quit(void);
static inline double get_dt(void);

/*!\brief largeur et hauteur de la fenêtre */
static int _wW = 800, _wH = 600;
/*!\brief identifiant du (futur) GLSL program */
static GLuint _pId = 0;
/*!\brief identifiant de la texture */
static GLuint _tex = 0;
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
    Vector3 autres_forces = {0.0, 0.0, 0.0};
    float k = 100.0;


GLuint textureId;

int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 20, 20, 
			 _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))
    return 1;
  init();
  atexit(quit);
  gl4duwDisplayFunc(draw);
  gl4duwKeyDownFunc(key);
  gl4duwMainLoop();
  return 0;
}



Vector3** tableau_principal;

void ap_tableau_poids(float* tableau_1D, Poids* tableau, int size) {
    for (int i = 0; i < size; i++) {
        tableau_1D[i * 3] = tableau[i].position.x*8;
        tableau_1D[i * 3 + 1] = tableau[i].position.y*8;
        tableau_1D[i * 3 + 2] = tableau[i].position.z*8;
        //printf("aaaaaaaaaaaaaaaaaaaaaa %f      aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa ",tableau_1D[i+2]);
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

GLuint createVertexTexture(Vector3** data, int width, int height) {
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Convertir le tableau 2D en un tableau 1D contenant les données consécutives
    float* flattenedData = (float*)malloc(width * height * 3 * sizeof(float));
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int index = (row * width + col) * 3;
            flattenedData[index] = data[row][col].x;
            //printf("%f, ",flattenedData[index]);
            flattenedData[index + 1] = data[row][col].y;
            //printf("%f, ",flattenedData[index+1]);
            flattenedData[index + 2] = data[row][col].z;
            //printf("%f, ",flattenedData[index+2]);
        }
    }

// Étape 3 : Configuration des paramètres de texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); // Définit le mode de wrapping sur l'axe des x
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Définit le mode de wrapping sur l'axe des y


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, flattenedData);
    free(flattenedData);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}


void init(void) {
  _pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
  _objId = gl4dgGenGrid2df (10, 10);
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

  tableau_principal = init_table(10, 10);
  createVertexTexture(tableau_principal, 10, 10);
  
  
      // Initialisation des valeurs des poids
    tableau_poids[0].position.x = 0.0;
    tableau_poids[0].position.y = 0.0;
    tableau_poids[0].position.z = 1.0;
    tableau_poids[0].masse = 1.0;
    tableau_poids[0].vitesse_instantanee.x = 0.0;
    tableau_poids[0].vitesse_instantanee.y = 0.0;
    tableau_poids[0].vitesse_instantanee.z = 0.0;

    tableau_poids[1].position.x = 1.0;
    tableau_poids[1].position.y = 0.0;
    tableau_poids[1].position.z = 1.0;
    tableau_poids[1].masse = 0.5;
    tableau_poids[1].vitesse_instantanee.x = 0.0;
    tableau_poids[1].vitesse_instantanee.y = 0.0;
    tableau_poids[1].vitesse_instantanee.z = 0.0;

    tableau_poids[2].position.x = 0.0;
    tableau_poids[2].position.y = 0.0;
    tableau_poids[2].position.z = 0.0;
    tableau_poids[2].masse = 0.5;
    tableau_poids[2].vitesse_instantanee.x = 0.0;
    tableau_poids[2].vitesse_instantanee.y = 0.0;
    tableau_poids[2].vitesse_instantanee.z = 0.0;

    tableau_poids[3].position.x = 1.0;
    tableau_poids[3].position.y = 0.0;
    tableau_poids[3].position.z = 0.0;
    tableau_poids[3].masse = 0.5;
    tableau_poids[3].vitesse_instantanee.x = 0.0;
    tableau_poids[3].vitesse_instantanee.y = 0.0;
    tableau_poids[3].vitesse_instantanee.z = 0.0;  
  
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


void draw(void) {
  GLfloat Lp[] = { -2.0f, 2.f, 0.0f, 1.0f };
  float* flattenedData = (float*)malloc(taille_tableau_poids  * 3 * sizeof(float));
  static float dt = 0;
  static float t = 0;

  dt = get_dt();

    
    
  static GLfloat angle = 0.0f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Lp[0] = -4.0f * cos(angle * M_PI / (1.0f * 180.0f));
  GLfloat bleu[] = {0, 0, 1, 1};
  GLfloat dir[] = {0, 0, 1};
  glUseProgram(_pId);

  gl4duBindMatrix("viewMatrix");

  gl4duLoadIdentityf();

  gl4duLookAtf(15, 15, 20, 0, 0, 0, 0, 1, 0);

  gl4duBindMatrix("modelMatrix");
  gl4duLoadIdentityf();

  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
  
  
  if (t>0.1){
  /*MAJ TAB*/
   calculer_forces_totale_maj_vitesses(tableau_poids, taille_tableau_poids, tableau_ressorts, taille_tableau_ressorts, longueur_ressort_repos, viscosite, rayon, autres_forces, k);

    // Affichage des résultats après l'appel de la fonction
    for (int i = 0; i < taille_tableau_poids; i++) {
        printf("Poids %d : vitesse = (%f, %f, %f)\n", i, tableau_poids[i].vitesse_instantanee.x, tableau_poids[i].vitesse_instantanee.y, tableau_poids[i].vitesse_instantanee.z);
    }
    maj_positions(tableau_poids, 2);


    tableau_poids[0].position.x = 0.0;
    tableau_poids[0].position.y = 0.0;
    tableau_poids[0].position.z = 1.0;
    tableau_poids[1].vitesse_instantanee.x = 0.0;
    afficher_positions_tableau(tableau_poids, 4);
    //afficher_positions_tableau(tableau_poids, 2);
    
/*Fin maj*/
ap_tableau_poids(flattenedData, tableau_poids, 4);
  
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 2, 2, 0, GL_RGB, GL_FLOAT, flattenedData);
	 t=0;
	 }else{
	   t +=dt;
	 }
	 
  glUniform1i(glGetUniformLocation(_pId, "textureSampler"),0);
  
  glUniform1f(glGetUniformLocation(_pId, "phase"), -angle / 180.0f);
  glUniform4fv(glGetUniformLocation(_pId, "Lp"), 1, Lp);
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu);

  gl4dgDraw(_objId);


  glUseProgram(0);
  free(flattenedData);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void quit(void) {

  free(tableau_principal);

  gl4duClean(GL4DU_ALL);
}

double get_dt(void) {
  static double t0 = 0.0;
  double t = gl4dGetElapsedTime() / 1000.0, dt = t - t0;
  t0 = t;
  return dt;
}
