
#include "app/app.h"

#include "mesh/mesh.h"
#include "texture/texture.h"
#include "museum.h"

int main(void) {
    App TPApp;
    
    TPApp.meshes = loadMuseum();

    TPApp.start();
}