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

double worldSize = 300.0;
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

// Block system globals
int blockSize = 30;
int roadWidth = 10;
int cityGridSize = 8;  // Will create a 9x9 grid (-4 to +4)

std::vector<CityBlock> cityBlocks;
std::vector<Building> buildings;
std::vector<StreetLamp> streetLamps;
std::vector<AmbientObject> ambientObjects;
std::vector<Tree> trees;
std::vector<Bench> benches;

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
  glEnable(GL_LIGHT0); // Reserved for minimal moonlight
  
  // Very dark ambient - just barely enough to see shapes
  float ambient[] = {0.03f, 0.03f, 0.05f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  
  // Enable additional lights
  // LIGHT0: Moonlight
  // LIGHT1-LIGHT6: Street lamps (6 closest to player)
  // LIGHT7: Player personal light
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);
  glEnable(GL_LIGHT4);
  glEnable(GL_LIGHT5);
  glEnable(GL_LIGHT6);
  glEnable(GL_LIGHT7);
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
  // Very minimal moonlight from above - just enough to see silhouettes
  float moonR = 0.05f;
  float moonG = 0.05f;
  float moonB = 0.08f;
  
  float lightPos[] = {0.0f, 100.0f, 0.0f, 1.0f}; // Directly overhead
  float lightColor[] = {moonR, moonG, moonB, 1.0f};
  
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
}

void setupStreetLampLights() {
  // Find the 6 closest working street lamps to the player
  // We have 8 lights total: LIGHT0=moon, LIGHT1-6=lamps, LIGHT7=player
  
  const int MAX_LAMP_LIGHTS = 6;
  int closestLamps[MAX_LAMP_LIGHTS];
  float closestDistances[MAX_LAMP_LIGHTS];
  
  // Initialize with invalid indices and max distance
  for (int i = 0; i < MAX_LAMP_LIGHTS; i++) {
    closestLamps[i] = -1;
    closestDistances[i] = 1000000.0f;
  }
  
  // Find closest working lamps
  for (size_t lampIdx = 0; lampIdx < streetLamps.size(); lampIdx++) {
    const StreetLamp& lamp = streetLamps[lampIdx];
    
    // Skip broken lamps
    if (!lamp.isWorking) continue;
    
    // Calculate distance to player
    float dx = lamp.x - playerX;
    float dz = lamp.z - playerZ;
    float dist = sqrt(dx*dx + dz*dz);
    
    // Check if this lamp is closer than any in our list
    for (int i = 0; i < MAX_LAMP_LIGHTS; i++) {
      if (dist < closestDistances[i]) {
        // Shift everything down
        for (int j = MAX_LAMP_LIGHTS - 1; j > i; j--) {
          closestLamps[j] = closestLamps[j-1];
          closestDistances[j] = closestDistances[j-1];
        }
        // Insert this lamp
        closestLamps[i] = lampIdx;
        closestDistances[i] = dist;
        break;
      }
    }
  }
  
  // Setup lights for the closest lamps (LIGHT1-LIGHT6)
  for (int i = 0; i < MAX_LAMP_LIGHTS; i++) {
    GLenum lightNum = GL_LIGHT1 + i;
    
    if (closestLamps[i] >= 0) {
      const StreetLamp& lamp = streetLamps[closestLamps[i]];
      
      // Calculate flicker - much slower now (0.5 instead of 10.0)
      float flicker = 0.7f + sin(timeOfDay * 0.5 + lamp.flickerPhase) * 0.3f * flickerIntensity;
      flicker = fmax(0.3f, fmin(1.0f, flicker));
      
      // Warm orange/yellow street lamp color
      float lampR = 0.9f * flicker;
      float lampG = 0.6f * flicker;
      float lampB = 0.2f * flicker;
      
      float lightPos[] = {(float)lamp.x, (float)(lamp.height + 0.4f), (float)lamp.z, 1.0f};
      float lightColor[] = {lampR, lampG, lampB, 1.0f};
      
      // Setup attenuation for falloff
      glLightfv(lightNum, GL_POSITION, lightPos);
      glLightfv(lightNum, GL_DIFFUSE, lightColor);
      glLightfv(lightNum, GL_SPECULAR, lightColor);
      glLightf(lightNum, GL_CONSTANT_ATTENUATION, 1.0f);
      glLightf(lightNum, GL_LINEAR_ATTENUATION, 0.05f);
      glLightf(lightNum, GL_QUADRATIC_ATTENUATION, 0.01f);
    } else {
      // No lamp for this light, turn it off
      float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
      glLightfv(lightNum, GL_DIFFUSE, black);
      glLightfv(lightNum, GL_SPECULAR, black);
    }
  }
  
  // Setup player personal light (LIGHT7) - follows the player
  float playerLightPos[] = {(float)playerX, (float)playerY, (float)playerZ, 1.0f};
  
  // Soft white/blue light - like a flashlight or personal lantern
  float playerLightColor[] = {0.4f, 0.4f, 0.5f, 1.0f};
  
  glLightfv(GL_LIGHT7, GL_POSITION, playerLightPos);
  glLightfv(GL_LIGHT7, GL_DIFFUSE, playerLightColor);
  glLightfv(GL_LIGHT7, GL_SPECULAR, playerLightColor);
  
  // Moderate falloff - illuminates area around player
  glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 1.0f);
  glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.1f);
  glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, 0.02f);
}

int main(int argc, char* argv[]) {
  srand(static_cast<unsigned int>(time(nullptr)));
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Eerie City - PS1 Horror Aesthetic [Block-Based]");

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.05f, 0.04f, 0.08f, 1.0f);

  std::cout << "\n=== Initializing Eerie City (Block-Based) ===" << std::endl;
  std::cout << "Block size: " << blockSize << " units" << std::endl;
  std::cout << "Road width: " << roadWidth << " units" << std::endl;
  std::cout << "City grid: " << (cityGridSize + 1) << "x" << (cityGridSize + 1) << " blocks" << std::endl;
  std::cout << std::endl;
  
  initializeCityGrid();
  generateRoadLights();
  initializeAmbientObjects();
  initializeLighting();
  initializeFog();
  
  std::cout << "\n=== City Generation Complete ===" << std::endl;
   
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  glutMainLoop();
  
  return 0;
}
