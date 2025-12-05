#ifndef EERIE_CITY_H
#define EERIE_CITY_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Platform-specific OpenGL includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================================
// PLAYER AND CAMERA GLOBALS
// ============================================================================

extern double playerX;
extern double playerZ;
extern double playerY;
extern double playerAngle;
extern double playerPitch;
extern double walkSpeed;
extern double turnSpeed;
extern double pitchSpeed;

// ============================================================================
// WORLD AND RENDERING SETTINGS
// ============================================================================

extern double worldSize;
extern double fogDensity;
extern double fov;

// ============================================================================
// TIME AND ATMOSPHERIC EFFECTS
// ============================================================================

extern double timeOfDay;
extern double daySpeed;
extern bool autoTime;
extern double flickerIntensity;
extern double noiseAmount;

// PS1-style dithering effect
extern bool ditherEnabled;
extern int ditherPattern[4][4];

// ============================================================================
// BLOCK SYSTEM SETTINGS
// ============================================================================

extern int blockSize;
extern int roadWidth;
extern int cityGridSize;

// ============================================================================
// ENUMERATIONS
// ============================================================================

// Block types for city generation
enum BlockType {
  BLOCK_EMPTY = 0,
  BLOCK_BUILDING = 1,
  BLOCK_PARK = 2,
  BLOCK_PLAZA = 3,
  BLOCK_INDUSTRIAL = 4,
  BLOCK_GRAVEYARD = 5,
  BLOCK_FOREST = 6
};

// Tree types for visual variety
enum TreeType {
  TREE_LAYERED = 0,    // Distinct foliage clusters with visible trunk
  TREE_DEAD = 1,       // Skeletal branches without leaves
  TREE_TWISTED = 2     // Irregular asymmetric branches with sparse foliage
};

// ============================================================================
// STRUCTURE DEFINITIONS
// ============================================================================

// City block structure - represents one grid cell in the city
struct CityBlock {
  int gridX, gridZ;                      // Grid coordinates
  double worldX, worldZ;                  // World position
  BlockType type;                         // Type of block
  std::vector<int> buildingIndices;       // References to buildings in this block
  std::vector<int> lampIndices;           // References to street lamps in this block
};

// Building structure - procedurally generated structures
struct Building {
  double x, z;                            // World position
  double width, depth, height;            // Dimensions
  double rotation;                        // Y-axis rotation in degrees
  float r, g, b;                          // Color
  int buildingType;                       // Visual style variation
  bool hasWindows;                        // Whether to render windows
  int windowPattern;                      // Window layout pattern
};

// Street lamp structure - light sources throughout the city
struct StreetLamp {
  double x, z;                            // World position
  double height;                          // Height of lamp post
  float flickerPhase;                     // Phase offset for flicker animation
  bool isWorking;                         // Whether lamp is functional
};

// Ambient object structure - environmental details
struct AmbientObject {
  double x, z;                            // World position
  double rotation;                        // Y-axis rotation
  int objectType;                         // Visual type (trash, debris, etc)
  float scale;                            // Size multiplier
};

// Tree structure - park and graveyard vegetation
struct Tree {
  double x, z;                            // World position
  double height;                          // Tree height
  float trunkR, trunkG, trunkB;           // Trunk color
  float leavesR, leavesG, leavesB;        // Foliage color
  float scale;                            // Size multiplier
  TreeType type;                          // Visual style
};

// Bench structure - park seating
struct Bench {
  double x, z;                            // World position
  double rotation;                        // Y-axis rotation
};

// Smokestack structure - industrial area features
struct Smokestack {
  double x, z;                            // World position
  double height;                          // Stack height
  float radius;                           // Base radius
};

// Chain-link fence structure - perimeter fencing
struct Fence {
  double x1, z1;                          // Start point
  double x2, z2;                          // End point
  double height;                          // Fence height
};

// Gravestone structure - cemetery markers
struct Gravestone {
  double x, z;                            // World position
  double width, height, depth;            // Dimensions
  double rotation;                        // Y-axis rotation
  int stoneType;                          // Visual style (0=cross, 1=rounded, 2=flat, 3=obelisk)
};

// Mausoleum structure - large cemetery structures
struct Mausoleum {
  double x, z;                            // World position
  double width, depth, height;            // Dimensions
  double rotation;                        // Y-axis rotation
};

// ============================================================================
// GLOBAL OBJECT VECTORS
// ============================================================================

extern std::vector<CityBlock> cityBlocks;
extern std::vector<Building> buildings;
extern std::vector<StreetLamp> streetLamps;
extern std::vector<AmbientObject> ambientObjects;
extern std::vector<Tree> trees;
extern std::vector<Bench> benches;
extern std::vector<Smokestack> smokestacks;
extern std::vector<Fence> fences;
extern std::vector<Gravestone> gravestones;
extern std::vector<Mausoleum> mausoleums;

// ============================================================================
// TEXTURE SYSTEM
// ============================================================================

// Texture IDs - Building and Ground
extern GLuint brickTexture;
extern GLuint concreteTexture;
extern GLuint roadTexture;
extern GLuint sidewalkTexture;
extern GLuint groundTexture;

// Texture IDs - Nature
extern GLuint leavesTexture;
extern GLuint barkTexture;

// Texture IDs - Metal and Lights
extern GLuint metalTexture;
extern GLuint lightTexture;
extern GLuint lampGlowTexture;

// Texture IDs - Road Markings
extern GLuint roadStripesTexture;

// Texture IDs - Structures
extern GLuint fenceTexture;
extern GLuint gravestoneTexture;

// Texture loading
void initializeTextures();
GLuint loadTexturePNG(const char* filename);

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void Print(const std::string& text);
void Fatal(const std::string& message);
void ErrCheck(const std::string& where);

// ============================================================================
// INITIALIZATION FUNCTIONS
// ============================================================================

void initializeLighting();
void updateLighting();
void setupStreetLampLights();
void initializeCityGrid();
void generateBuildingBlock(CityBlock& block);
void generateParkBlock(CityBlock& block);
void generateIndustrialBlock(CityBlock& block);
void generateGraveyardBlock(CityBlock& block);
void generateForestBlock(CityBlock& block);
void generateRoadLights();
void initializeAmbientObjects();
void initializeFog();

// ============================================================================
// DRAWING FUNCTIONS
// ============================================================================

void drawBuilding(const Building& building);
void drawStreetLamp(const StreetLamp& lamp);
void drawAmbientObject(const AmbientObject& obj);

// Tree drawing functions
void drawTree(const Tree& tree);          // Main dispatcher function
void drawLayeredTree(const Tree& tree);   // Layered foliage style
void drawDeadTree(const Tree& tree);      // Skeletal style
void drawTwistedTree(const Tree& tree);   // Twisted/gnarled style

void drawBench(const Bench& bench);
void drawSmokestack(const Smokestack& stack);
void drawFence(const Fence& fence);
void drawGravestone(const Gravestone& stone);
void drawMausoleum(const Mausoleum& mausoleum);
void drawGroundPlane();
void drawRoads();
void drawCityBlockSidewalks();
void drawSky();

// PS1 visual effects
void applyDitherEffect();
void applyScreenDistortion();

// ============================================================================
// CALLBACK FUNCTIONS
// ============================================================================

void display();
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void special(int key, int x, int y);
void idle();

#endif
