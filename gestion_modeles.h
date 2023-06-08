//#include "structs.h"
#include "forces.h"
#include <lib/cJSON.h>

Modele* create_modele() ;
void free_modele(Modele* model_ptr);
Modele* load_modele(const char* filename);
Env load_env(const char* filename);
Env create_env();