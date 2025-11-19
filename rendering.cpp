#include "eerie_city.h"

void drawBuilding(const Building& building) {
  glPushMatrix();
  glTranslated(building.x, 0.0, building.z);
  glRotated(building.rotation, 0.0, 1.0, 0.0);
  
  // Building body with vertex jitter for PS1 effect
  float jitter = 0.02f;
  glColor3f(building.r, building.g, building.b);
  
  glBegin(GL_QUADS);
  
  // Front face
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-building.width + ((rand() % 100) / 5000.0f - jitter), 0.0f, building.depth);
  glVertex3f(building.width + ((rand() % 100) / 5000.0f - jitter), 0.0f, building.depth);
  glVertex3f(building.width + ((rand() % 100) / 5000.0f - jitter), building.height, building.depth);
  glVertex3f(-building.width + ((rand() % 100) / 5000.0f - jitter), building.height, building.depth);
  
  // Back face
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(building.width, 0.0f, -building.depth);
  glVertex3f(-building.width, 0.0f, -building.depth);
  glVertex3f(-building.width, building.height, -building.depth);
  glVertex3f(building.width, building.height, -building.depth);
  
  // Right face
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(building.width, 0.0f, building.depth);
  glVertex3f(building.width, 0.0f, -building.depth);
  glVertex3f(building.width, building.height, -building.depth);
  glVertex3f(building.width, building.height, building.depth);
  
  // Left face
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(-building.width, 0.0f, -building.depth);
  glVertex3f(-building.width, 0.0f, building.depth);
  glVertex3f(-building.width, building.height, building.depth);
  glVertex3f(-building.width, building.height, -building.depth);
  
  // Top (flat roof)
  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-building.width, building.height, building.depth);
  glVertex3f(building.width, building.height, building.depth);
  glVertex3f(building.width, building.height, -building.depth);
  glVertex3f(-building.width, building.height, -building.depth);
  
  glEnd();
  
  // Windows with eerie glow - disable lighting so they emit light
  if (building.hasWindows) {
    glDisable(GL_LIGHTING);
    
    int windowsPerFloor = 2 + (building.windowPattern % 3);
    int numFloors = (int)(building.height / 3.0);
    
    for (int floor = 1; floor < numFloors; floor++) {
      for (int w = 0; w < windowsPerFloor; w++) {
        double windowY = floor * 3.0;
        double windowX = -building.width * 0.8 + (w * building.width * 1.6 / windowsPerFloor);
        
        // Use deterministic "random" based on position for consistent lit windows
        // This prevents flickering by giving same result each frame
        int windowSeed = (int)(building.x * 100 + building.z * 100 + floor * 10 + w);
        
        // Random chance of lit window (30%)
        if ((windowSeed % 100) < 30) {
          // Bright warm glow - much brighter than before
          float glowR = 1.0f;
          float glowG = 0.8f;
          float glowB = 0.4f;
          
          glColor3f(glowR, glowG, glowB);
        } else {
          // Dark unlit window - slightly visible
          glColor3f(0.08f, 0.08f, 0.12f);
        }
        
        glBegin(GL_QUADS);
        // Front windows
        glVertex3f(windowX - 0.4f, windowY - 0.3f, building.depth + 0.01f);
        glVertex3f(windowX + 0.4f, windowY - 0.3f, building.depth + 0.01f);
        glVertex3f(windowX + 0.4f, windowY + 0.6f, building.depth + 0.01f);
        glVertex3f(windowX - 0.4f, windowY + 0.6f, building.depth + 0.01f);
        glEnd();
      }
    }
    
    glEnable(GL_LIGHTING);
  }
  
  // Edge lines for definition
  glDisable(GL_LIGHTING);
  glColor3f(0.0f, 0.0f, 0.0f);
  glLineWidth(1.5f);
  glBegin(GL_LINES);
  
  // Vertical edges
  glVertex3f(-building.width, 0.0f, building.depth);
  glVertex3f(-building.width, building.height, building.depth);
  
  glVertex3f(building.width, 0.0f, building.depth);
  glVertex3f(building.width, building.height, building.depth);
  
  glVertex3f(building.width, 0.0f, -building.depth);
  glVertex3f(building.width, building.height, -building.depth);
  
  glVertex3f(-building.width, 0.0f, -building.depth);
  glVertex3f(-building.width, building.height, -building.depth);
  
  glEnd();
  glEnable(GL_LIGHTING);
  
  glPopMatrix();
}

void drawStreetLamp(const StreetLamp& lamp) {
  glPushMatrix();
  glTranslated(lamp.x, 0.0, lamp.z);
  
  // Lamp post - fully enclosed rectangular prism
  glColor3f(0.2f, 0.2f, 0.25f);
  glBegin(GL_QUADS);
  
  // Front face
  glVertex3f(-0.15f, 0.0f, 0.15f);
  glVertex3f(0.15f, 0.0f, 0.15f);
  glVertex3f(0.15f, lamp.height, 0.15f);
  glVertex3f(-0.15f, lamp.height, 0.15f);
  
  // Back face
  glVertex3f(0.15f, 0.0f, -0.15f);
  glVertex3f(-0.15f, 0.0f, -0.15f);
  glVertex3f(-0.15f, lamp.height, -0.15f);
  glVertex3f(0.15f, lamp.height, -0.15f);
  
  // Right face
  glVertex3f(0.15f, 0.0f, 0.15f);
  glVertex3f(0.15f, 0.0f, -0.15f);
  glVertex3f(0.15f, lamp.height, -0.15f);
  glVertex3f(0.15f, lamp.height, 0.15f);
  
  // Left face
  glVertex3f(-0.15f, 0.0f, -0.15f);
  glVertex3f(-0.15f, 0.0f, 0.15f);
  glVertex3f(-0.15f, lamp.height, 0.15f);
  glVertex3f(-0.15f, lamp.height, -0.15f);
  
  glEnd();
  
  // Lamp head - fully enclosed box
  if (lamp.isWorking) {
    // Much slower flicker - was multiplying timeOfDay by 10.0, now using 0.5
    float flicker = 0.7f + sin(timeOfDay * 0.5 + lamp.flickerPhase) * 0.3f * flickerIntensity;
    flicker = fmax(0.3f, fmin(1.0f, flicker));
    
    glDisable(GL_LIGHTING);
    glColor3f(0.9f * flicker, 0.7f * flicker, 0.4f * flicker);
    
    glBegin(GL_QUADS);
    
    float lampTop = lamp.height + 0.8f;
    
    // Front face
    glVertex3f(-0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    glVertex3f(-0.4f, lampTop, 0.4f);
    
    // Back face
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    
    // Right face
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    
    // Left face
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lamp.height, 0.4f);
    glVertex3f(-0.4f, lampTop, 0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    
    // Top face (glowing)
    glVertex3f(-0.4f, lampTop, 0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    
    // Bottom face
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(-0.4f, lamp.height, 0.4f);
    
    glEnd();
    
    glEnable(GL_LIGHTING);
  } else {
    // Broken lamp - dark enclosed box
    glColor3f(0.1f, 0.1f, 0.1f);
    
    glBegin(GL_QUADS);
    
    float lampTop = lamp.height + 0.8f;
    
    // Front face
    glVertex3f(-0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    glVertex3f(-0.4f, lampTop, 0.4f);
    
    // Back face
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    
    // Right face
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    
    // Left face
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lamp.height, 0.4f);
    glVertex3f(-0.4f, lampTop, 0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    
    // Top face
    glVertex3f(-0.4f, lampTop, 0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    
    // Bottom face
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(-0.4f, lamp.height, 0.4f);
    
    glEnd();
  }
  
  glPopMatrix();
}

void drawAmbientObject(const AmbientObject& obj) {
  glPushMatrix();
  glTranslated(obj.x, 0.0, obj.z);
  glRotated(obj.rotation, 0.0, 1.0, 0.0);
  glScaled(obj.scale, obj.scale, obj.scale);
  
  glColor3f(0.15f, 0.14f, 0.16f);
  
  switch(obj.objectType) {
    case 0: // Trash pile
      glBegin(GL_TRIANGLES);
      glVertex3f(0.0f, 0.8f, 0.0f);
      glVertex3f(-0.6f, 0.0f, 0.4f);
      glVertex3f(0.6f, 0.0f, 0.4f);
      glEnd();
      break;
      
    case 1: // Box
      glBegin(GL_QUADS);
      glVertex3f(-0.5f, 0.0f, -0.5f);
      glVertex3f(0.5f, 0.0f, -0.5f);
      glVertex3f(0.5f, 0.7f, -0.5f);
      glVertex3f(-0.5f, 0.7f, -0.5f);
      glEnd();
      break;
      
    case 2: // Debris
      glBegin(GL_TRIANGLES);
      glVertex3f(0.0f, 0.3f, 0.0f);
      glVertex3f(-0.4f, 0.0f, -0.3f);
      glVertex3f(0.4f, 0.0f, 0.3f);
      glEnd();
      break;
      
    case 3: // Low barrier
      glBegin(GL_QUADS);
      glVertex3f(-1.0f, 0.0f, -0.1f);
      glVertex3f(1.0f, 0.0f, -0.1f);
      glVertex3f(1.0f, 0.4f, -0.1f);
      glVertex3f(-1.0f, 0.4f, -0.1f);
      glEnd();
      break;
  }
  
  glPopMatrix();
}

void drawGroundPlane() {
  glColor3f(0.12f, 0.12f, 0.14f);
  
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3d(-worldSize, 0.0, -worldSize);
  glVertex3d(worldSize, 0.0, -worldSize);
  glVertex3d(worldSize, 0.0, worldSize);
  glVertex3d(-worldSize, 0.0, worldSize);
  glEnd();
}

void drawRoads() {
  glDisable(GL_LIGHTING);
  glDisable(GL_FOG);  // Disable fog for roads
  
  int halfGrid = cityGridSize / 2;
  int totalBlockSize = blockSize + roadWidth;
  
  // Bright unfogged road color
  glColor3f(0.20f, 0.20f, 0.22f);
  
  // Draw horizontal roads at base height
  for (int gx = -halfGrid; gx <= halfGrid + 1; gx++) {
    double roadCenterX = gx * totalBlockSize - roadWidth / 2.0;
    double roadStart = roadCenterX - roadWidth / 2.0;
    double roadEnd = roadCenterX + roadWidth / 2.0;
    
    glBegin(GL_QUADS);
    glVertex3d(roadStart, 0.15, -worldSize);
    glVertex3d(roadEnd, 0.15, -worldSize);
    glVertex3d(roadEnd, 0.15, worldSize);
    glVertex3d(roadStart, 0.15, worldSize);
    glEnd();
  }
  
  // Draw vertical roads slightly higher to avoid Z-fighting
  for (int gz = -halfGrid; gz <= halfGrid + 1; gz++) {
    double roadCenterZ = gz * totalBlockSize - roadWidth / 2.0;
    double roadStart = roadCenterZ - roadWidth / 2.0;
    double roadEnd = roadCenterZ + roadWidth / 2.0;
    
    glBegin(GL_QUADS);
    glVertex3d(-worldSize, 0.16, roadStart);
    glVertex3d(worldSize, 0.16, roadStart);
    glVertex3d(worldSize, 0.16, roadEnd);
    glVertex3d(-worldSize, 0.16, roadEnd);
    glEnd();
  }
  
  // Draw road center lines and markings (raised above road surface)
  glColor3f(0.30f, 0.30f, 0.32f);  // Even brighter markings
  glLineWidth(1.5f);
  glBegin(GL_LINES);
  
  // Horizontal road center lines
  for (int gx = -halfGrid; gx <= halfGrid + 1; gx++) {
    double roadCenterX = gx * totalBlockSize - roadWidth / 2.0;
    
    // Dashed center line
    for (double z = -worldSize; z < worldSize; z += 8.0) {
      float offset = sin(z * 0.05f) * 0.3f;
      glVertex3d(roadCenterX + offset, 0.2, z);
      glVertex3d(roadCenterX + offset, 0.2, z + 4.0);
    }
  }
  
  // Vertical road center lines
  for (int gz = -halfGrid; gz <= halfGrid + 1; gz++) {
    double roadCenterZ = gz * totalBlockSize - roadWidth / 2.0;
    
    // Dashed center line
    for (double x = -worldSize; x < worldSize; x += 8.0) {
      float offset = sin(x * 0.05f) * 0.3f;
      glVertex3d(x, 0.2, roadCenterZ + offset);
      glVertex3d(x + 4.0, 0.2, roadCenterZ + offset);
    }
  }
  
  glEnd();
  glEnable(GL_FOG);  // Re-enable fog for other objects
  glEnable(GL_LIGHTING);
}

void drawSky() {
  // Save current state
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  
  // Switch to ortho projection for 2D drawing
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  // Draw full-screen gradient quad
  glBegin(GL_QUADS);
  
  // Top (darker)
  glColor3f(0.02f, 0.02f, 0.05f);
  glVertex2f(-1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);
  
  // Bottom (slightly lighter)
  glColor3f(0.08f, 0.06f, 0.12f);
  glVertex2f(1.0f, -1.0f);
  glVertex2f(-1.0f, -1.0f);
  
  glEnd();
  
  // Restore matrices
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  // Restore state
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_FOG);
}

void applyScreenDistortion() {
  // PS1-style vertex snapping simulation
  // This would ideally be done in vertex shader, but we simulate with slight vertex jitter
  glLineWidth(1.0f);
}

void applyDitherEffect() {
  if (!ditherEnabled) return;
  
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  // Apply dither pattern
  int ditherSize = 4;
  glPointSize(1.0f);
  glBegin(GL_POINTS);
  
  for (int x = 0; x < viewport[2]; x += ditherSize) {
    for (int y = 0; y < viewport[3]; y += ditherSize) {
      int patternX = x % 4;
      int patternY = y % 4;
      float alpha = ditherPattern[patternY][patternX] / 32.0f * noiseAmount;
      
      glColor4f(0.0f, 0.0f, 0.0f, alpha);
      glVertex2i(x, y);
    }
  }
  
  glEnd();
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
}

void drawTree(const Tree& tree) {
  glPushMatrix();
  glTranslated(tree.x, 0.0, tree.z);
  glScaled(tree.scale, tree.scale, tree.scale);
  
  // Draw trunk - fully enclosed rectangular prism
  glColor3f(tree.trunkR, tree.trunkG, tree.trunkB);
  glBegin(GL_QUADS);
  
  float trunkHeight = tree.height * 0.5f;
  
  // Front face
  glVertex3f(-0.3f, 0.0f, 0.3f);
  glVertex3f(0.3f, 0.0f, 0.3f);
  glVertex3f(0.3f, trunkHeight, 0.3f);
  glVertex3f(-0.3f, trunkHeight, 0.3f);
  
  // Back face
  glVertex3f(0.3f, 0.0f, -0.3f);
  glVertex3f(-0.3f, 0.0f, -0.3f);
  glVertex3f(-0.3f, trunkHeight, -0.3f);
  glVertex3f(0.3f, trunkHeight, -0.3f);
  
  // Right face
  glVertex3f(0.3f, 0.0f, 0.3f);
  glVertex3f(0.3f, 0.0f, -0.3f);
  glVertex3f(0.3f, trunkHeight, -0.3f);
  glVertex3f(0.3f, trunkHeight, 0.3f);
  
  // Left face
  glVertex3f(-0.3f, 0.0f, -0.3f);
  glVertex3f(-0.3f, 0.0f, 0.3f);
  glVertex3f(-0.3f, trunkHeight, 0.3f);
  glVertex3f(-0.3f, trunkHeight, -0.3f);
  
  glEnd();
  
  // Draw more rounded, spherical foliage - multiple layers creating sphere-like shape
  glColor3f(tree.leavesR, tree.leavesG, tree.leavesB);
  
  float leafBase = tree.height * 0.35f;
  float leafLow = tree.height * 0.5f;
  float leafMid = tree.height * 0.7f;
  float leafHigh = tree.height * 0.85f;
  float leafTop = tree.height * 1.0f;
  
  // Radius decreases as we go up for rounded shape
  float radiusBottom = 2.2f;
  float radiusLow = 2.0f;
  float radiusMid = 1.6f;
  float radiusHigh = 1.0f;
  
  glBegin(GL_TRIANGLES);
  
  // BOTTOM SECTION - octagonal bottom face (not flat underneath)
  // Bottom pyramid pointing down from leafBase
  glVertex3f(0.0f, leafBase - 0.5f, 0.0f); // Point below
  glVertex3f(radiusBottom, leafBase, 0.0f);
  glVertex3f(radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  
  glVertex3f(0.0f, leafBase - 0.5f, 0.0f);
  glVertex3f(radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  glVertex3f(0.0f, leafBase, radiusBottom);
  
  glVertex3f(0.0f, leafBase - 0.5f, 0.0f);
  glVertex3f(0.0f, leafBase, radiusBottom);
  glVertex3f(-radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  
  glVertex3f(0.0f, leafBase - 0.5f, 0.0f);
  glVertex3f(-radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  glVertex3f(-radiusBottom, leafBase, 0.0f);
  
  glVertex3f(0.0f, leafBase - 0.5f, 0.0f);
  glVertex3f(-radiusBottom, leafBase, 0.0f);
  glVertex3f(-radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  
  glVertex3f(0.0f, leafBase - 0.5f, 0.0f);
  glVertex3f(-radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  glVertex3f(0.0f, leafBase, -radiusBottom);
  
  glVertex3f(0.0f, leafBase - 0.5f, 0.0f);
  glVertex3f(0.0f, leafBase, -radiusBottom);
  glVertex3f(radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  
  glVertex3f(0.0f, leafBase - 0.5f, 0.0f);
  glVertex3f(radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  glVertex3f(radiusBottom, leafBase, 0.0f);
  
  // LOWER RING - base to low (8 faces for roundness)
  // Front-right
  glVertex3f(radiusBottom, leafBase, 0.0f);
  glVertex3f(radiusLow, leafLow, 0.0f);
  glVertex3f(radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  
  glVertex3f(radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  glVertex3f(radiusLow, leafLow, 0.0f);
  glVertex3f(radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  
  // Right
  glVertex3f(radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  glVertex3f(radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  glVertex3f(0.0f, leafBase, radiusBottom);
  
  glVertex3f(0.0f, leafBase, radiusBottom);
  glVertex3f(radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  glVertex3f(0.0f, leafLow, radiusLow);
  
  // Back-right
  glVertex3f(0.0f, leafBase, radiusBottom);
  glVertex3f(0.0f, leafLow, radiusLow);
  glVertex3f(-radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  
  glVertex3f(-radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  glVertex3f(0.0f, leafLow, radiusLow);
  glVertex3f(-radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  
  // Back
  glVertex3f(-radiusBottom * 0.7f, leafBase, radiusBottom * 0.7f);
  glVertex3f(-radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  glVertex3f(-radiusBottom, leafBase, 0.0f);
  
  glVertex3f(-radiusBottom, leafBase, 0.0f);
  glVertex3f(-radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  glVertex3f(-radiusLow, leafLow, 0.0f);
  
  // Back-left
  glVertex3f(-radiusBottom, leafBase, 0.0f);
  glVertex3f(-radiusLow, leafLow, 0.0f);
  glVertex3f(-radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  
  glVertex3f(-radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  glVertex3f(-radiusLow, leafLow, 0.0f);
  glVertex3f(-radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  
  // Left
  glVertex3f(-radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  glVertex3f(-radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  glVertex3f(0.0f, leafBase, -radiusBottom);
  
  glVertex3f(0.0f, leafBase, -radiusBottom);
  glVertex3f(-radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  glVertex3f(0.0f, leafLow, -radiusLow);
  
  // Front-left
  glVertex3f(0.0f, leafBase, -radiusBottom);
  glVertex3f(0.0f, leafLow, -radiusLow);
  glVertex3f(radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  
  glVertex3f(radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  glVertex3f(0.0f, leafLow, -radiusLow);
  glVertex3f(radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  
  // Front
  glVertex3f(radiusBottom * 0.7f, leafBase, -radiusBottom * 0.7f);
  glVertex3f(radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  glVertex3f(radiusBottom, leafBase, 0.0f);
  
  glVertex3f(radiusBottom, leafBase, 0.0f);
  glVertex3f(radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  glVertex3f(radiusLow, leafLow, 0.0f);
  
  glEnd();
  
  // Continue with simpler geometry for middle and top
  glColor3f(tree.leavesR * 0.9f, tree.leavesG * 0.9f, tree.leavesB * 0.9f);
  glBegin(GL_TRIANGLES);
  
  // MIDDLE RING - 8 sided
  glVertex3f(radiusLow, leafLow, 0.0f);
  glVertex3f(radiusMid, leafMid, 0.0f);
  glVertex3f(radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  
  glVertex3f(radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  glVertex3f(radiusMid, leafMid, 0.0f);
  glVertex3f(radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  
  glVertex3f(radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  glVertex3f(radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  glVertex3f(0.0f, leafLow, radiusLow);
  
  glVertex3f(0.0f, leafLow, radiusLow);
  glVertex3f(radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  glVertex3f(0.0f, leafMid, radiusMid);
  
  glVertex3f(0.0f, leafLow, radiusLow);
  glVertex3f(0.0f, leafMid, radiusMid);
  glVertex3f(-radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  
  glVertex3f(-radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  glVertex3f(0.0f, leafMid, radiusMid);
  glVertex3f(-radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  
  glVertex3f(-radiusLow * 0.7f, leafLow, radiusLow * 0.7f);
  glVertex3f(-radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  glVertex3f(-radiusLow, leafLow, 0.0f);
  
  glVertex3f(-radiusLow, leafLow, 0.0f);
  glVertex3f(-radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  glVertex3f(-radiusMid, leafMid, 0.0f);
  
  glVertex3f(-radiusLow, leafLow, 0.0f);
  glVertex3f(-radiusMid, leafMid, 0.0f);
  glVertex3f(-radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  
  glVertex3f(-radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  glVertex3f(-radiusMid, leafMid, 0.0f);
  glVertex3f(-radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  
  glVertex3f(-radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  glVertex3f(-radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  glVertex3f(0.0f, leafLow, -radiusLow);
  
  glVertex3f(0.0f, leafLow, -radiusLow);
  glVertex3f(-radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  glVertex3f(0.0f, leafMid, -radiusMid);
  
  glVertex3f(0.0f, leafLow, -radiusLow);
  glVertex3f(0.0f, leafMid, -radiusMid);
  glVertex3f(radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  
  glVertex3f(radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  glVertex3f(0.0f, leafMid, -radiusMid);
  glVertex3f(radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  
  glVertex3f(radiusLow * 0.7f, leafLow, -radiusLow * 0.7f);
  glVertex3f(radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  glVertex3f(radiusLow, leafLow, 0.0f);
  
  glVertex3f(radiusLow, leafLow, 0.0f);
  glVertex3f(radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  glVertex3f(radiusMid, leafMid, 0.0f);
  
  glEnd();
  
  // TOP - simple cap to finish the sphere
  glColor3f(tree.leavesR * 0.8f, tree.leavesG * 0.8f, tree.leavesB * 0.8f);
  glBegin(GL_TRIANGLES);
  
  // 8 triangles converging to top point
  glVertex3f(0.0f, leafTop, 0.0f);
  glVertex3f(radiusMid, leafMid, 0.0f);
  glVertex3f(radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  
  glVertex3f(0.0f, leafTop, 0.0f);
  glVertex3f(radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  glVertex3f(0.0f, leafMid, radiusMid);
  
  glVertex3f(0.0f, leafTop, 0.0f);
  glVertex3f(0.0f, leafMid, radiusMid);
  glVertex3f(-radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  
  glVertex3f(0.0f, leafTop, 0.0f);
  glVertex3f(-radiusMid * 0.7f, leafMid, radiusMid * 0.7f);
  glVertex3f(-radiusMid, leafMid, 0.0f);
  
  glVertex3f(0.0f, leafTop, 0.0f);
  glVertex3f(-radiusMid, leafMid, 0.0f);
  glVertex3f(-radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  
  glVertex3f(0.0f, leafTop, 0.0f);
  glVertex3f(-radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  glVertex3f(0.0f, leafMid, -radiusMid);
  
  glVertex3f(0.0f, leafTop, 0.0f);
  glVertex3f(0.0f, leafMid, -radiusMid);
  glVertex3f(radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  
  glVertex3f(0.0f, leafTop, 0.0f);
  glVertex3f(radiusMid * 0.7f, leafMid, -radiusMid * 0.7f);
  glVertex3f(radiusMid, leafMid, 0.0f);
  
  glEnd();
  
  // Add some branch geometry for more detail
  glColor3f(tree.trunkR * 1.1f, tree.trunkG * 1.1f, tree.trunkB * 1.1f);
  glBegin(GL_TRIANGLES);
  
  // Simple branch stubs sticking out
  float branchHeight = tree.height * 0.45f;
  
  // Branch pointing forward
  glVertex3f(0.0f, branchHeight, 0.0f);
  glVertex3f(-0.2f, branchHeight + 0.1f, 1.0f);
  glVertex3f(0.2f, branchHeight + 0.1f, 1.0f);
  
  // Branch pointing right
  glVertex3f(0.0f, branchHeight + 0.3f, 0.0f);
  glVertex3f(1.0f, branchHeight + 0.4f, -0.2f);
  glVertex3f(1.0f, branchHeight + 0.4f, 0.2f);
  
  // Branch pointing left
  glVertex3f(0.0f, branchHeight + 0.2f, 0.0f);
  glVertex3f(-1.0f, branchHeight + 0.3f, 0.2f);
  glVertex3f(-1.0f, branchHeight + 0.3f, -0.2f);
  
  glEnd();
  
  glPopMatrix();
}

void drawBench(const Bench& bench) {
  glPushMatrix();
  glTranslated(bench.x, 0.0, bench.z);
  glRotated(bench.rotation, 0.0, 1.0, 0.0);
  
  // Dark weathered wood color
  glColor3f(0.18f, 0.16f, 0.15f);
  
  // Bench seat
  glBegin(GL_QUADS);
  glVertex3f(-1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.5f, -0.3f);
  glVertex3f(-1.0f, 0.5f, -0.3f);
  
  glVertex3f(-1.0f, 0.4f, 0.3f);
  glVertex3f(1.0f, 0.4f, 0.3f);
  glVertex3f(1.0f, 0.5f, 0.3f);
  glVertex3f(-1.0f, 0.5f, 0.3f);
  glEnd();
  
  // Bench back
  glBegin(GL_QUADS);
  glVertex3f(-1.0f, 0.5f, -0.35f);
  glVertex3f(1.0f, 0.5f, -0.35f);
  glVertex3f(1.0f, 1.2f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.35f);
  glEnd();
  
  // Simple legs (4 posts)
  glBegin(GL_QUADS);
  // Left front leg
  glVertex3f(-0.8f, 0.0f, 0.2f);
  glVertex3f(-0.7f, 0.0f, 0.2f);
  glVertex3f(-0.7f, 0.4f, 0.2f);
  glVertex3f(-0.8f, 0.4f, 0.2f);
  
  // Right front leg
  glVertex3f(0.7f, 0.0f, 0.2f);
  glVertex3f(0.8f, 0.0f, 0.2f);
  glVertex3f(0.8f, 0.4f, 0.2f);
  glVertex3f(0.7f, 0.4f, 0.2f);
  
  // Left back leg
  glVertex3f(-0.8f, 0.0f, -0.2f);
  glVertex3f(-0.7f, 0.0f, -0.2f);
  glVertex3f(-0.7f, 0.4f, -0.2f);
  glVertex3f(-0.8f, 0.4f, -0.2f);
  
  // Right back leg
  glVertex3f(0.7f, 0.0f, -0.2f);
  glVertex3f(0.8f, 0.0f, -0.2f);
  glVertex3f(0.8f, 0.4f, -0.2f);
  glVertex3f(0.7f, 0.4f, -0.2f);
  glEnd();
  
  glPopMatrix();
}
