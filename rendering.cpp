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
        
        int windowSeed = (int)(building.x * 100 + building.z * 100 + floor * 10 + w);
        
        if ((windowSeed % 100) < 30) {
          float glowR = 1.0f;
          float glowG = 0.8f;
          float glowB = 0.4f;
          
          glColor3f(glowR, glowG, glowB);
        } else {
          glColor3f(0.08f, 0.08f, 0.12f);
        }
        
        glBegin(GL_QUADS);
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
  
  glColor3f(0.2f, 0.2f, 0.25f);
  glBegin(GL_QUADS);
  
  glVertex3f(-0.15f, 0.0f, 0.15f);
  glVertex3f(0.15f, 0.0f, 0.15f);
  glVertex3f(0.15f, lamp.height, 0.15f);
  glVertex3f(-0.15f, lamp.height, 0.15f);
  
  glVertex3f(0.15f, 0.0f, -0.15f);
  glVertex3f(-0.15f, 0.0f, -0.15f);
  glVertex3f(-0.15f, lamp.height, -0.15f);
  glVertex3f(0.15f, lamp.height, -0.15f);
  
  glVertex3f(0.15f, 0.0f, 0.15f);
  glVertex3f(0.15f, 0.0f, -0.15f);
  glVertex3f(0.15f, lamp.height, -0.15f);
  glVertex3f(0.15f, lamp.height, 0.15f);
  
  glVertex3f(-0.15f, 0.0f, -0.15f);
  glVertex3f(-0.15f, 0.0f, 0.15f);
  glVertex3f(-0.15f, lamp.height, 0.15f);
  glVertex3f(-0.15f, lamp.height, -0.15f);
  
  glEnd();
  
  if (lamp.isWorking) {
    float flicker = 0.7f + sin(timeOfDay * 0.5 + lamp.flickerPhase) * 0.3f * flickerIntensity;
    flicker = fmax(0.3f, fmin(1.0f, flicker));
    
    glDisable(GL_LIGHTING);
    glColor3f(0.9f * flicker, 0.7f * flicker, 0.4f * flicker);
    
    glBegin(GL_QUADS);
    
    float lampTop = lamp.height + 0.8f;
    
    glVertex3f(-0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    glVertex3f(-0.4f, lampTop, 0.4f);
    
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lamp.height, 0.4f);
    glVertex3f(-0.4f, lampTop, 0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    
    glVertex3f(-0.4f, lampTop, 0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(-0.4f, lamp.height, 0.4f);
    
    glEnd();
    
    glEnable(GL_LIGHTING);
  } else {
    glColor3f(0.1f, 0.1f, 0.1f);
    
    glBegin(GL_QUADS);
    
    float lampTop = lamp.height + 0.8f;
    
    glVertex3f(-0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    glVertex3f(-0.4f, lampTop, 0.4f);
    
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    
    glVertex3f(0.4f, lamp.height, 0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(-0.4f, lamp.height, 0.4f);
    glVertex3f(-0.4f, lampTop, 0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    
    glVertex3f(-0.4f, lampTop, 0.4f);
    glVertex3f(0.4f, lampTop, 0.4f);
    glVertex3f(0.4f, lampTop, -0.4f);
    glVertex3f(-0.4f, lampTop, -0.4f);
    
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
    case 0:
      glBegin(GL_TRIANGLES);
      glVertex3f(0.0f, 0.8f, 0.0f);
      glVertex3f(-0.6f, 0.0f, 0.4f);
      glVertex3f(0.6f, 0.0f, 0.4f);
      glEnd();
      break;
      
    case 1:
      glBegin(GL_QUADS);
      glVertex3f(-0.5f, 0.0f, -0.5f);
      glVertex3f(0.5f, 0.0f, -0.5f);
      glVertex3f(0.5f, 0.7f, -0.5f);
      glVertex3f(-0.5f, 0.7f, -0.5f);
      glEnd();
      break;
      
    case 2:
      glBegin(GL_TRIANGLES);
      glVertex3f(0.0f, 0.3f, 0.0f);
      glVertex3f(-0.4f, 0.0f, -0.3f);
      glVertex3f(0.4f, 0.0f, 0.3f);
      glEnd();
      break;
      
    case 3:
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
  glDisable(GL_FOG);
  
  int halfGrid = cityGridSize / 2;
  int totalBlockSize = blockSize + roadWidth;
  
  glColor3f(0.20f, 0.20f, 0.22f);
  
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
  
  glColor3f(0.30f, 0.30f, 0.32f);
  glLineWidth(1.5f);
  glBegin(GL_LINES);
  
  for (int gx = -halfGrid; gx <= halfGrid + 1; gx++) {
    double roadCenterX = gx * totalBlockSize - roadWidth / 2.0;
    
    for (double z = -worldSize; z < worldSize; z += 8.0) {
      float offset = sin(z * 0.05f) * 0.3f;
      glVertex3d(roadCenterX + offset, 0.2, z);
      glVertex3d(roadCenterX + offset, 0.2, z + 4.0);
    }
  }
  
  for (int gz = -halfGrid; gz <= halfGrid + 1; gz++) {
    double roadCenterZ = gz * totalBlockSize - roadWidth / 2.0;
    
    for (double x = -worldSize; x < worldSize; x += 8.0) {
      float offset = sin(x * 0.05f) * 0.3f;
      glVertex3d(x, 0.2, roadCenterZ + offset);
      glVertex3d(x + 4.0, 0.2, roadCenterZ + offset);
    }
  }
  
  glEnd();
  glEnable(GL_FOG);
  glEnable(GL_LIGHTING);
}

void drawSky() {
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  glBegin(GL_QUADS);
  
  glColor3f(0.02f, 0.02f, 0.04f);
  glVertex2f(-1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);
  
  glColor3f(0.05f, 0.04f, 0.08f);
  glVertex2f(1.0f, -1.0f);
  glVertex2f(-1.0f, -1.0f);
  
  glEnd();
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_FOG);
}

void applyScreenDistortion() {
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

void drawRoundedCanopyCluster(float baseHeight, float topHeight, float maxRadius, int segments) {
  glBegin(GL_TRIANGLES);
  
  float height = topHeight - baseHeight;
  int numRings = 5;
  
  for (int ring = 0; ring < numRings; ring++) {
    float t1 = ring / (float)numRings;
    float t2 = (ring + 1) / (float)numRings;
    
    float h1 = baseHeight + t1 * height;
    float h2 = baseHeight + t2 * height;
    
    float radius1 = maxRadius * sin(t1 * M_PI);
    float radius2 = maxRadius * sin(t2 * M_PI);
    
    for (int i = 0; i < segments; i++) {
      float angle1 = (i / (float)segments) * 2.0f * M_PI;
      float angle2 = ((i + 1) / (float)segments) * 2.0f * M_PI;
      
      float variation1 = 0.95f + (i % 3) * 0.025f;
      float variation2 = 0.95f + ((i + 1) % 3) * 0.025f;
      
      float x1_r1 = cos(angle1) * radius1 * variation1;
      float z1_r1 = sin(angle1) * radius1 * variation1;
      float x2_r1 = cos(angle2) * radius1 * variation2;
      float z2_r1 = sin(angle2) * radius1 * variation2;
      
      float x1_r2 = cos(angle1) * radius2 * variation1;
      float z1_r2 = sin(angle1) * radius2 * variation1;
      float x2_r2 = cos(angle2) * radius2 * variation2;
      float z2_r2 = sin(angle2) * radius2 * variation2;
      
      if (radius1 > 0.01f && radius2 > 0.01f) {
        glVertex3f(x1_r1, h1, z1_r1);
        glVertex3f(x2_r1, h1, z2_r1);
        glVertex3f(x1_r2, h2, z1_r2);
        
        glVertex3f(x2_r1, h1, z2_r1);
        glVertex3f(x2_r2, h2, z2_r2);
        glVertex3f(x1_r2, h2, z1_r2);
      } else if (radius1 > 0.01f) {
        glVertex3f(x1_r1, h1, z1_r1);
        glVertex3f(x2_r1, h1, z2_r1);
        glVertex3f(0.0f, h2, 0.0f);
      } else if (radius2 > 0.01f) {
        glVertex3f(0.0f, h1, 0.0f);
        glVertex3f(x1_r2, h2, z1_r2);
        glVertex3f(x2_r2, h2, z2_r2);
      }
    }
  }
  
  glEnd();
}

void drawLayeredTree(const Tree& tree) {
  glPushMatrix();
  glTranslated(tree.x, 0.0, tree.z);
  glScaled(tree.scale, tree.scale, tree.scale);
  
  // Draw foliage clusters FIRST
  float bottomBase = tree.height * 0.30f;
  float bottomTop = tree.height * 0.48f;
  glColor3f(tree.leavesR, tree.leavesG, tree.leavesB);
  drawRoundedCanopyCluster(bottomBase, bottomTop, 1.5f, 8);
  
  float middleBase = tree.height * 0.55f;
  float middleTop = tree.height * 0.70f;
  glColor3f(tree.leavesR * 0.95f, tree.leavesG * 0.95f, tree.leavesB * 0.95f);
  drawRoundedCanopyCluster(middleBase, middleTop, 1.1f, 8);
  
  float topBase = tree.height * 0.75f;
  float topTop = tree.height * 0.92f;
  glColor3f(tree.leavesR * 0.9f, tree.leavesG * 0.9f, tree.leavesB * 0.9f);
  drawRoundedCanopyCluster(topBase, topTop, 0.7f, 6);
  
  // Small cap cluster at the very top
  float capBase = tree.height * 0.93f;
  float capTop = tree.height * 1.0f;
  glColor3f(tree.leavesR * 0.85f, tree.leavesG * 0.85f, tree.leavesB * 0.85f);
  drawRoundedCanopyCluster(capBase, capTop, 0.4f, 5);
  
  // Draw trunk AFTER foliage so it renders on top
  glColor3f(tree.trunkR, tree.trunkG, tree.trunkB);
  glBegin(GL_QUADS);
  
  float trunkHeight = tree.height * 0.95f;
  float trunkBaseWidth = 0.5f;
  float trunkTopWidth = 0.12f;
  
  for (int i = 0; i < 8; i++) {
    float angle1 = (i / 8.0f) * 2.0f * M_PI;
    float angle2 = ((i + 1) / 8.0f) * 2.0f * M_PI;
    
    float x1_base = cos(angle1) * trunkBaseWidth;
    float z1_base = sin(angle1) * trunkBaseWidth;
    float x2_base = cos(angle2) * trunkBaseWidth;
    float z2_base = sin(angle2) * trunkBaseWidth;
    
    float x1_top = cos(angle1) * trunkTopWidth;
    float z1_top = sin(angle1) * trunkTopWidth;
    float x2_top = cos(angle2) * trunkTopWidth;
    float z2_top = sin(angle2) * trunkTopWidth;
    
    glVertex3f(x1_base, 0.0f, z1_base);
    glVertex3f(x2_base, 0.0f, z2_base);
    glVertex3f(x2_top, trunkHeight, z2_top);
    glVertex3f(x1_top, trunkHeight, z1_top);
  }
  
  glEnd();
  
  // Draw branch stubs in the gap areas
  glColor3f(tree.trunkR * 1.2f, tree.trunkG * 1.2f, tree.trunkB * 1.2f);
  glBegin(GL_TRIANGLES);
  
  float branchHeights[] = {tree.height * 0.50f, tree.height * 0.72f};
  
  for (int heightIdx = 0; heightIdx < 2; heightIdx++) {
    float branchHeight = branchHeights[heightIdx];
    int numBranches = 3 + (heightIdx % 2);
    
    for (int i = 0; i < numBranches; i++) {
      float angle = (i / (float)numBranches) * 2.0f * M_PI;
      float bx = cos(angle) * 1.2f;
      float bz = sin(angle) * 1.2f;
      
      glVertex3f(0.0f, branchHeight, 0.0f);
      glVertex3f(bx * 0.3f, branchHeight + 0.1f, bz * 0.3f);
      glVertex3f(bx, branchHeight - 0.1f, bz);
    }
  }
  
  glEnd();
  glPopMatrix();
}

void drawDeadTree(const Tree& tree) {
  glPushMatrix();
  glTranslated(tree.x, 0.0, tree.z);
  glScaled(tree.scale, tree.scale, tree.scale);
  
  glColor3f(tree.trunkR * 0.8f, tree.trunkG * 0.8f, tree.trunkB * 0.8f);
  glBegin(GL_QUADS);
  
  float trunkHeight = tree.height * 0.95f;
  float trunkBaseWidth = 0.4f;
  float trunkTopWidth = 0.1f;
  
  for (int i = 0; i < 6; i++) {
    float angle1 = (i / 6.0f) * 2.0f * M_PI;
    float angle2 = ((i + 1) / 6.0f) * 2.0f * M_PI;
    
    float x1_base = cos(angle1) * trunkBaseWidth;
    float z1_base = sin(angle1) * trunkBaseWidth;
    float x2_base = cos(angle2) * trunkBaseWidth;
    float z2_base = sin(angle2) * trunkBaseWidth;
    
    float x1_top = cos(angle1) * trunkTopWidth;
    float z1_top = sin(angle1) * trunkTopWidth;
    float x2_top = cos(angle2) * trunkTopWidth;
    float z2_top = sin(angle2) * trunkTopWidth;
    
    glVertex3f(x1_base, 0.0f, z1_base);
    glVertex3f(x2_base, 0.0f, z2_base);
    glVertex3f(x2_top, trunkHeight, z2_top);
    glVertex3f(x1_top, trunkHeight, z1_top);
  }
  
  glEnd();
  
  glColor3f(tree.trunkR, tree.trunkG, tree.trunkB);
  glBegin(GL_TRIANGLES);
  
  for (int level = 0; level < 5; level++) {
    float heightRatio = 0.3f + (level * 0.15f);
    float branchHeight = tree.height * heightRatio;
    int numBranches = 5 + (level % 3);
    float branchLength = 2.0f - (level * 0.3f);
    
    for (int i = 0; i < numBranches; i++) {
      float angle = (i / (float)numBranches) * 2.0f * M_PI + (level * 0.4f);
      float bx = cos(angle) * branchLength;
      float bz = sin(angle) * branchLength;
      
      glVertex3f(0.0f, branchHeight, 0.0f);
      glVertex3f(bx * 0.2f, branchHeight + 0.3f, bz * 0.2f);
      glVertex3f(bx, branchHeight - 0.2f, bz);
      
      if (level < 4) {
        float subAngle = angle + 0.5f;
        float subLength = branchLength * 0.5f;
        float sbx = cos(subAngle) * subLength;
        float sbz = sin(subAngle) * subLength;
        
        glVertex3f(bx * 0.5f, branchHeight - 0.1f, bz * 0.5f);
        glVertex3f(bx * 0.6f, branchHeight + 0.1f, bz * 0.6f);
        glVertex3f(sbx, branchHeight - 0.3f, sbz);
      }
    }
  }
  
  int topSpikes = 6;
  float crownHeight = tree.height * 0.9f;
  for (int i = 0; i < topSpikes; i++) {
    float angle = (i / (float)topSpikes) * 2.0f * M_PI;
    float cx = cos(angle) * 0.8f;
    float cz = sin(angle) * 0.8f;
    
    glVertex3f(0.0f, tree.height, 0.0f);
    glVertex3f(cx * 0.3f, crownHeight + 0.2f, cz * 0.3f);
    glVertex3f(cx, crownHeight - 0.3f, cz);
  }
  
  glEnd();
  glPopMatrix();
}

void drawTwistedTree(const Tree& tree) {
  glPushMatrix();
  glTranslated(tree.x, 0.0, tree.z);
  glScaled(tree.scale, tree.scale, tree.scale);
  
  // Draw sparse foliage clusters FIRST - 8 total distributed along branches
  glColor3f(tree.leavesR, tree.leavesG, tree.leavesB);
  
  // 8 clusters at various heights (lower to higher)
  float clusterAngles[] = {0.2f, 1.0f, 1.8f, 2.6f, 3.4f, 4.2f, 5.0f, 5.8f};
  float clusterHeights[] = {0.40f, 0.48f, 0.56f, 0.64f, 0.72f, 0.78f, 0.84f, 0.88f};  // 8 positions from 40% to 88%
  
  for (int i = 0; i < 8; i++) {
    float angle = clusterAngles[i];
    float heightRatio = clusterHeights[i];
    float clusterHeight = tree.height * heightRatio;
    float clusterDist = 1.0f + (i % 3) * 0.3f;  // Vary distance
    
    float cx = cos(angle) * clusterDist;
    float cz = sin(angle) * clusterDist;
    
    glPushMatrix();
    glTranslatef(cx, clusterHeight, cz);
    
    // Vary cluster sizes - smaller at top
    float clusterSize = 0.7f - (i * 0.05f);  // Gets smaller as we go up
    drawRoundedCanopyCluster(-0.3f, 0.4f, clusterSize, 6);
    
    glPopMatrix();
  }
  
  // Draw twisted trunk AFTER foliage - tapers to SHARP POINT at top
  glColor3f(tree.trunkR, tree.trunkG, tree.trunkB);
  glBegin(GL_QUADS);
  
  float trunkHeight = tree.height * 0.98f;  // Almost full height
  
  int trunkSections = 10;  // More sections for smoother taper to point
  for (int section = 0; section < trunkSections; section++) {
    float t1 = section / (float)trunkSections;
    float t2 = (section + 1) / (float)trunkSections;
    
    float h1 = t1 * trunkHeight;
    float h2 = t2 * trunkHeight;
    
    // Aggressive taper to sharp point (near zero at top)
    float w1 = 0.5f * (1.0f - t1 * 0.95f);  // Very aggressive taper
    float w2 = 0.5f * (1.0f - t2 * 0.95f);  // Gets to ~0.025 at top
    
    float offset1X = sin(t1 * M_PI * 2.0f) * 0.3f;
    float offset1Z = cos(t1 * M_PI * 1.5f) * 0.2f;
    float offset2X = sin(t2 * M_PI * 2.0f) * 0.3f;
    float offset2Z = cos(t2 * M_PI * 1.5f) * 0.2f;
    
    for (int i = 0; i < 6; i++) {
      float angle1 = (i / 6.0f) * 2.0f * M_PI;
      float angle2 = ((i + 1) / 6.0f) * 2.0f * M_PI;
      
      float x1_b = cos(angle1) * w1 + offset1X;
      float z1_b = sin(angle1) * w1 + offset1Z;
      float x2_b = cos(angle2) * w1 + offset1X;
      float z2_b = sin(angle2) * w1 + offset1Z;
      
      float x1_t = cos(angle1) * w2 + offset2X;
      float z1_t = sin(angle1) * w2 + offset2Z;
      float x2_t = cos(angle2) * w2 + offset2X;
      float z2_t = sin(angle2) * w2 + offset2Z;
      
      glVertex3f(x1_b, h1, z1_b);
      glVertex3f(x2_b, h1, z2_b);
      glVertex3f(x2_t, h2, z2_t);
      glVertex3f(x1_t, h2, z1_t);
    }
  }
  
  glEnd();
  
  // Draw gnarled asymmetric branches - more branches to support more foliage
  glColor3f(tree.trunkR * 1.1f, tree.trunkG * 1.1f, tree.trunkB * 1.1f);
  glBegin(GL_TRIANGLES);
  
  float branchAngles[] = {0.3f, 1.1f, 1.9f, 2.7f, 3.5f, 4.3f, 5.1f, 5.9f};
  float branchHeights[] = {0.38f, 0.46f, 0.54f, 0.62f, 0.70f, 0.76f, 0.82f, 0.86f};
  
  for (int i = 0; i < 8; i++) {
    float angle = branchAngles[i];
    float heightRatio = branchHeights[i];
    float branchHeight = tree.height * heightRatio;
    float branchLength = 1.4f + (i % 3) * 0.25f;
    
    float bx = cos(angle) * branchLength;
    float bz = sin(angle) * branchLength;
    
    // Main branch
    glVertex3f(0.0f, branchHeight, 0.0f);
    glVertex3f(bx * 0.4f, branchHeight - 0.2f, bz * 0.4f);
    glVertex3f(bx, branchHeight + 0.1f, bz);
    
    // Branch fork
    float forkAngle = angle + 0.6f;
    float fx = cos(forkAngle) * branchLength * 0.6f;
    float fz = sin(forkAngle) * branchLength * 0.6f;
    
    glVertex3f(bx * 0.5f, branchHeight, bz * 0.5f);
    glVertex3f(bx * 0.6f, branchHeight + 0.15f, bz * 0.6f);
    glVertex3f(fx, branchHeight + 0.2f, fz);
  }
  
  glEnd();
  
  glPopMatrix();
}

void drawTree(const Tree& tree) {
  switch(tree.type) {
    case TREE_LAYERED:
      drawLayeredTree(tree);
      break;
    case TREE_DEAD:
      drawDeadTree(tree);
      break;
    case TREE_TWISTED:
      drawTwistedTree(tree);
      break;
    default:
      drawLayeredTree(tree);
      break;
  }
}

void drawBench(const Bench& bench) {
  glPushMatrix();
  glTranslated(bench.x, 0.0, bench.z);
  glRotated(bench.rotation, 0.0, 1.0, 0.0);
  
  glColor3f(0.18f, 0.16f, 0.15f);
  
  glBegin(GL_QUADS);
  
  glVertex3f(-1.0f, 0.5f, -0.3f);
  glVertex3f(1.0f, 0.5f, -0.3f);
  glVertex3f(1.0f, 0.5f, 0.3f);
  glVertex3f(-1.0f, 0.5f, 0.3f);
  
  glVertex3f(-1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, 0.3f);
  glVertex3f(-1.0f, 0.4f, 0.3f);
  
  glVertex3f(-1.0f, 0.4f, 0.3f);
  glVertex3f(1.0f, 0.4f, 0.3f);
  glVertex3f(1.0f, 0.5f, 0.3f);
  glVertex3f(-1.0f, 0.5f, 0.3f);
  
  glVertex3f(-1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.5f, -0.3f);
  glVertex3f(-1.0f, 0.5f, -0.3f);
  
  glVertex3f(-1.0f, 0.4f, -0.3f);
  glVertex3f(-1.0f, 0.4f, 0.3f);
  glVertex3f(-1.0f, 0.5f, 0.3f);
  glVertex3f(-1.0f, 0.5f, -0.3f);
  
  glVertex3f(1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, 0.3f);
  glVertex3f(1.0f, 0.5f, 0.3f);
  glVertex3f(1.0f, 0.5f, -0.3f);
  
  glEnd();
  
  glBegin(GL_QUADS);
  
  glVertex3f(-1.0f, 0.5f, -0.35f);
  glVertex3f(1.0f, 0.5f, -0.35f);
  glVertex3f(1.0f, 1.2f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.35f);
  
  glVertex3f(-1.0f, 0.5f, -0.45f);
  glVertex3f(1.0f, 0.5f, -0.45f);
  glVertex3f(1.0f, 1.2f, -0.45f);
  glVertex3f(-1.0f, 1.2f, -0.45f);
  
  glVertex3f(-1.0f, 1.2f, -0.45f);
  glVertex3f(1.0f, 1.2f, -0.45f);
  glVertex3f(1.0f, 1.2f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.35f);
  
  glVertex3f(-1.0f, 0.5f, -0.45f);
  glVertex3f(-1.0f, 0.5f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.45f);
  
  glVertex3f(1.0f, 0.5f, -0.45f);
  glVertex3f(1.0f, 0.5f, -0.35f);
  glVertex3f(1.0f, 1.2f, -0.35f);
  glVertex3f(1.0f, 1.2f, -0.45f);
  
  glEnd();
  
  glBegin(GL_QUADS);
  
  glVertex3f(-0.8f, 0.0f, 0.25f);
  glVertex3f(-0.7f, 0.0f, 0.25f);
  glVertex3f(-0.7f, 0.4f, 0.25f);
  glVertex3f(-0.8f, 0.4f, 0.25f);
  glVertex3f(-0.8f, 0.0f, 0.15f);
  glVertex3f(-0.7f, 0.0f, 0.15f);
  glVertex3f(-0.7f, 0.4f, 0.15f);
  glVertex3f(-0.8f, 0.4f, 0.15f);
  glVertex3f(-0.8f, 0.0f, 0.15f);
  glVertex3f(-0.8f, 0.0f, 0.25f);
  glVertex3f(-0.8f, 0.4f, 0.25f);
  glVertex3f(-0.8f, 0.4f, 0.15f);
  glVertex3f(-0.7f, 0.0f, 0.15f);
  glVertex3f(-0.7f, 0.0f, 0.25f);
  glVertex3f(-0.7f, 0.4f, 0.25f);
  glVertex3f(-0.7f, 0.4f, 0.15f);
  
  glVertex3f(0.7f, 0.0f, 0.25f);
  glVertex3f(0.8f, 0.0f, 0.25f);
  glVertex3f(0.8f, 0.4f, 0.25f);
  glVertex3f(0.7f, 0.4f, 0.25f);
  glVertex3f(0.7f, 0.0f, 0.15f);
  glVertex3f(0.8f, 0.0f, 0.15f);
  glVertex3f(0.8f, 0.4f, 0.15f);
  glVertex3f(0.7f, 0.4f, 0.15f);
  glVertex3f(0.7f, 0.0f, 0.15f);
  glVertex3f(0.7f, 0.0f, 0.25f);
  glVertex3f(0.7f, 0.4f, 0.25f);
  glVertex3f(0.7f, 0.4f, 0.15f);
  glVertex3f(0.8f, 0.0f, 0.15f);
  glVertex3f(0.8f, 0.0f, 0.25f);
  glVertex3f(0.8f, 0.4f, 0.25f);
  glVertex3f(0.8f, 0.4f, 0.15f);
  
  glVertex3f(-0.8f, 0.0f, -0.15f);
  glVertex3f(-0.7f, 0.0f, -0.15f);
  glVertex3f(-0.7f, 0.4f, -0.15f);
  glVertex3f(-0.8f, 0.4f, -0.15f);
  glVertex3f(-0.8f, 0.0f, -0.25f);
  glVertex3f(-0.7f, 0.0f, -0.25f);
  glVertex3f(-0.7f, 0.4f, -0.25f);
  glVertex3f(-0.8f, 0.4f, -0.25f);
  glVertex3f(-0.8f, 0.0f, -0.25f);
  glVertex3f(-0.8f, 0.0f, -0.15f);
  glVertex3f(-0.8f, 0.4f, -0.15f);
  glVertex3f(-0.8f, 0.4f, -0.25f);
  glVertex3f(-0.7f, 0.0f, -0.25f);
  glVertex3f(-0.7f, 0.0f, -0.15f);
  glVertex3f(-0.7f, 0.4f, -0.15f);
  glVertex3f(-0.7f, 0.4f, -0.25f);
  
  glVertex3f(0.7f, 0.0f, -0.15f);
  glVertex3f(0.8f, 0.0f, -0.15f);
  glVertex3f(0.8f, 0.4f, -0.15f);
  glVertex3f(0.7f, 0.4f, -0.15f);
  glVertex3f(0.7f, 0.0f, -0.25f);
  glVertex3f(0.8f, 0.0f, -0.25f);
  glVertex3f(0.8f, 0.4f, -0.25f);
  glVertex3f(0.7f, 0.4f, -0.25f);
  glVertex3f(0.7f, 0.0f, -0.25f);
  glVertex3f(0.7f, 0.0f, -0.15f);
  glVertex3f(0.7f, 0.4f, -0.15f);
  glVertex3f(0.7f, 0.4f, -0.25f);
  glVertex3f(0.8f, 0.0f, -0.25f);
  glVertex3f(0.8f, 0.0f, -0.15f);
  glVertex3f(0.8f, 0.4f, -0.15f);
  glVertex3f(0.8f, 0.4f, -0.25f);
  
  glEnd();
  
  glPopMatrix();
}
