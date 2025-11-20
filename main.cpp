#include "eerie_city.h"

// ============================================================================
// GLOBAL VARIABLE DEFINITIONS
// ============================================================================

// Player/Camera state
double playerX = 0.0;
double playerZ = 0.0;
double playerY = 2.0;
double playerAngle = 0.0;
double playerPitch = 0.0;
double walkSpeed = 0.4;
double turnSpeed = 2.5;
double pitchSpeed = 2.0;

// World bounds and rendering
double worldSize = 300.0;
double fogDensity = 0.025;
double fov = 70.0;

// Time and atmospheric effects
double timeOfDay = 22.0;
double daySpeed = 0.01;
bool autoTime = true;
double flickerIntensity = 1.0;
double noiseAmount = 0.03;

// PS1-style dithering effect
bool ditherEnabled = true;
int ditherPattern[4][4] = {
  {0, 8, 2, 10},
  {12, 4, 14, 6},
  {3, 11, 1, 9},
  {15, 7, 13, 5}
};

// Block system configuration
int blockSize = 30;
int roadWidth = 10;
int cityGridSize = 8;  // Creates a 9x9 grid (-4 to +4)

// World object collections
std::vector<CityBlock> cityBlocks;
std::vector<Building> buildings;
std::vector<StreetLamp> streetLamps;
std::vector<AmbientObject> ambientObjects;
std::vector<Tree> trees;
std::vector<Bench> benches;
std::vector<Smokestack> smokestacks;
std::vector<Fence> fences;
std::vector<Gravestone> gravestones;
std::vector<Mausoleum> mausoleums;

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Print text using GLUT bitmap font
void Print(const std::string& text) {
  for (char ch : text) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ch);
  }
}

// Print fatal error message and exit
void Fatal(const std::string& message) {
  std::cerr << "FATAL ERROR: " << message << '\n';
  exit(1);
}

// Check for OpenGL errors
void ErrCheck(const std::string& where) {
  int err = glGetError();
  if (err) {
    std::cerr << "ERROR: " << gluErrorString(err) << " [" << where << "]" << '\n';
  }
}

// ============================================================================
// LIGHTING SYSTEM
// ============================================================================

void initializeLighting() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0); // Reserved for minimal moonlight
  
  // Stronger ambient light for omnidirectional visibility
  float ambient[] = {0.15f, 0.15f, 0.18f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  
  // Enable two-sided lighting so backfaces are lit
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  
  // Enable additional lights:
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
  glFogi(GL_FOG_MODE, GL_EXP2);  // Exponential squared for sharp falloff
  
  // Dark, oppressive fog for horror atmosphere
  float fogColor[] = {0.05f, 0.04f, 0.08f, 1.0f};
  glFogfv(GL_FOG_COLOR, fogColor);
  
  // Dense fog for ~50ft (15 units) visibility with sharp cutoff
  glFogf(GL_FOG_DENSITY, 0.025f);
  
  glHint(GL_FOG_HINT, GL_NICEST);
}

void updateLighting() {
  // Moonlight for distant visibility
  float moonR = 0.08f;
  float moonG = 0.08f;
  float moonB = 0.12f;
  
  // Position moon at an angle for dramatic shadows
  float lightPos[] = {50.0f, 80.0f, -30.0f, 1.0f};
  float lightColor[] = {moonR, moonG, moonB, 1.0f};
  
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
  
  // Add attenuation so moonlight is weaker up close
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.001f);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00001f);
}

void setupStreetLampLights() {
  // Find the 6 closest working street lamps to the player
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
      
      // Calculate flicker effect
      float flicker = 0.7f + sin(timeOfDay * 0.5 + lamp.flickerPhase) * 0.3f * flickerIntensity;
      flicker = fmax(0.3f, fmin(1.0f, flicker));
      
      // Warm orange/yellow street lamp color
      float lampR = 0.9f * flicker;
      float lampG = 0.6f * flicker;
      float lampB = 0.2f * flicker;
      
      float lightPos[] = {(float)lamp.x, (float)(lamp.height + 0.4f), (float)lamp.z, 1.0f};
      float lightColor[] = {lampR, lampG, lampB, 1.0f};
      
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
  
  // Setup player personal light (LIGHT7)
  float playerLightPos[] = {(float)playerX, (float)playerY, (float)playerZ, 1.0f};
  
  // Bright light with strong ambient component for omnidirectional effect
  float playerLightDiffuse[] = {0.8f, 0.8f, 0.9f, 1.0f};
  float playerLightAmbient[] = {0.4f, 0.4f, 0.45f, 1.0f};
  
  glLightfv(GL_LIGHT7, GL_POSITION, playerLightPos);
  glLightfv(GL_LIGHT7, GL_DIFFUSE, playerLightDiffuse);
  glLightfv(GL_LIGHT7, GL_AMBIENT, playerLightAmbient);
  glLightfv(GL_LIGHT7, GL_SPECULAR, playerLightDiffuse);
  
  // Wider range for ~25ft clear visibility
  glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 1.0f);
  glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.02f);
  glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, 0.003f);
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

int main(int argc, char* argv[]) {
  srand(static_cast<unsigned int>(time(nullptr)));
  
  // Initialize GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Eerie City - PS1 Horror Aesthetic [Block-Based]");

  // OpenGL state setup
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.05f, 0.04f, 0.08f, 1.0f);

  std::cout << "\n=== Initializing Eerie City (Block-Based) ===" << std::endl;
  std::cout << "Block size: " << blockSize << " units" << std::endl;
  std::cout << "Road width: " << roadWidth << " units" << std::endl;
  std::cout << "City grid: " << (cityGridSize + 1) << "x" << (cityGridSize + 1) << " blocks" << std::endl;
  std::cout << std::endl;
  
  // Generate world
  initializeCityGrid();
  generateRoadLights();
  initializeAmbientObjects();
  initializeLighting();
  initializeFog();
  
  std::cout << "\n=== City Generation Complete ===" << std::endl;
  
  // Register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  
  // Start main loop
  glutMainLoop();
  
  return 0;
}
