#ifndef EERIE_CITY_H
#define EERIE_CITY_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

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

// Player/Camera globals
extern double playerX;
extern double playerZ;
extern double playerY;
extern double playerAngle;
extern double playerPitch;
extern double walkSpeed;
extern double turnSpeed;
extern double pitchSpeed;

// World settings
extern double worldSize;
extern double fogDensity;

// View settings
extern double fov;

// Time and atmospheric effects
extern double timeOfDay;
extern double daySpeed;
extern bool autoTime;
extern double flickerIntensity;
extern double noiseAmount;

// Dithering effect
extern bool ditherEnabled;
extern int ditherPattern[4][4];

// Block system settings
extern int blockSize;
extern int roadWidth;
extern int cityGridSize;

// Block types
enum BlockType {
  BLOCK_EMPTY = 0,
  BLOCK_BUILDING = 1,
  BLOCK_PARK = 2,
  BLOCK_PLAZA = 3
};

// City block structure
struct CityBlock {
  int gridX, gridZ;
  double worldX, worldZ;
  BlockType type;
  std::vector<int> buildingIndices;  // Indices into buildings vector
  std::vector<int> lampIndices;      // Indices into streetLamps vector
};

// Building structure
struct Building {
  double x, z;
  double width, depth, height;
  double rotation;
  float r, g, b;
  int buildingType;
  bool hasWindows;
  int windowPattern;
};

// Street lamp structure
struct StreetLamp {
  double x, z;
  double height;
  float flickerPhase;
  bool isWorking;
};

// Ambient object structure (trash, debris, etc)
struct AmbientObject {
  double x, z;
  double rotation;
  int objectType;
  float scale;
};

// Tree structure for parks
struct Tree {
  double x, z;
  double height;
  float trunkR, trunkG, trunkB;
  float leavesR, leavesG, leavesB;
  float scale;
};

// Bench structure for parks
struct Bench {
  double x, z;
  double rotation;
};

extern std::vector<CityBlock> cityBlocks;
extern std::vector<Building> buildings;
extern std::vector<StreetLamp> streetLamps;
extern std::vector<AmbientObject> ambientObjects;
extern std::vector<Tree> trees;
extern std::vector<Bench> benches;

// Function declarations
void Print(const std::string& text);
void Fatal(const std::string& message);
void ErrCheck(const std::string& where);

// Initialization functions
void initializeLighting();
void updateLighting();
void setupStreetLampLights();
void initializeCityGrid();
void generateBuildingBlock(CityBlock& block);
void generateParkBlock(CityBlock& block);
void generateRoadLights();
void generateRoadLights();
void initializeAmbientObjects();
void initializeFog();

// Drawing functions
void drawBuilding(const Building& building);
void drawStreetLamp(const StreetLamp& lamp);
void drawAmbientObject(const AmbientObject& obj);
void drawTree(const Tree& tree);
void drawBench(const Bench& bench);
void drawGroundPlane();
void drawRoads();
void drawSky();
void applyDitherEffect();
void applyScreenDistortion();

// Callback functions
void display();
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void special(int key, int x, int y);
void idle();

#endif
