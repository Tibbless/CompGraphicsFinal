#include "eerie_city.h"

// Global variable definitions
double playerX = 0.0;
double playerZ = 0.0;
double playerY = 2.0;
double playerAngle = 0.0;
double playerPitch = 0.0;
double walkSpeed = 0.4;
double turnSpeed = 2.5;
double pitchSpeed = 2.0;

double worldSize = 200.0;
double fogDensity = 0.008;
double fov = 70.0;

double timeOfDay = 22.0;
double daySpeed = 0.01;
bool autoTime = true;
double flickerIntensity = 1.0;
double noiseAmount = 0.03;

bool ditherEnabled = true;
int ditherPattern[4][4] = {
  {0, 8, 2, 10},
  {12, 4, 14, 6},
  {3, 11, 1, 9},
  {15, 7, 13, 5}
};

std::vector<Building> buildings;
std::vector<StreetLamp> streetLamps;
std::vector<AmbientObject> ambientObjects;

void Print(const std::string& text) {
  for (char ch : text) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ch);
  }
}

void Fatal(const std::string& message) {
  std::cerr << "FATAL ERROR: " << message << '\n';
  exit(1);
}

void ErrCheck(const std::string& where) {
  int err = glGetError();
  if (err) {
    std::cerr << "ERROR: " << gluErrorString(err) << " [" << where << "]" << '\n';
  }
}

void initializeLighting() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  float ambient[] = {0.15f, 0.12f, 0.18f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void initializeFog() {
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_EXP2);
  
  float fogColor[] = {0.08f, 0.06f, 0.12f, 1.0f};
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_DENSITY, fogDensity);
  glHint(GL_FOG_HINT, GL_DONT_CARE);
}

void updateLighting() {
  // Night time lighting with slight variations
  float nightR = 0.15f + sin(timeOfDay * 0.1f) * 0.05f;
  float nightG = 0.12f + cos(timeOfDay * 0.15f) * 0.03f;
  float nightB = 0.25f + sin(timeOfDay * 0.2f) * 0.08f;
  
  float lightPos[] = {50.0f, 80.0f, 50.0f, 1.0f};
  float lightColor[] = {nightR, nightG, nightB, 1.0f};
  
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
}

int main(int argc, char* argv[]) {
  srand(static_cast<unsigned int>(time(nullptr)));
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Eerie City - PS1 Horror Aesthetic");

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.05f, 0.04f, 0.08f, 1.0f);

  initializeBuildings();
  initializeLighting();
  initializeFog();
  initializeStreetLamps();
  initializeAmbientObjects();
   
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  glutMainLoop();
  
  return 0;
}