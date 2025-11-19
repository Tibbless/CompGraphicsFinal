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
  
  // Top (very dark to match thick fog)
  glColor3f(0.02f, 0.02f, 0.04f);
  glVertex2f(-1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);
  
  // Bottom (matches fog color for seamless blend)
  glColor3f(0.05f, 0.04f, 0.08f);
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

// Helper function to draw a rounded egg-shaped canopy cluster for trees
void drawRoundedCanopyCluster(float baseHeight, float topHeight, float maxRadius, int segments) {
  glBegin(GL_TRIANGLES);
  
  float centerHeight = (baseHeight + topHeight) / 2.0f;
  float height = topHeight - baseHeight;
  
  // Create multiple horizontal rings to form rounded shape
  int numRings = 5;
  
  for (int ring = 0; ring < numRings; ring++) {
    float t1 = ring / (float)numRings;
    float t2 = (ring + 1) / (float)numRings;
    
    // Calculate heights
    float h1 = baseHeight + t1 * height;
    float h2 = baseHeight + t2 * height;
    
    // Calculate radius using a rounded curve (wider in middle, narrow at ends)
    // Using sine curve for egg shape: wide in middle, tapered at top and bottom
    float radius1 = maxRadius * sin(t1 * M_PI);  // 0 at bottom, max at middle, 0 at top
    float radius2 = maxRadius * sin(t2 * M_PI);
    
    // Add slight asymmetry
    for (int i = 0; i < segments; i++) {
      float angle1 = (i / (float)segments) * 2.0f * M_PI;
      float angle2 = ((i + 1) / (float)segments) * 2.0f * M_PI;
      
      // Slight variation for organic look
      float variation1 = 0.95f + (i % 3) * 0.025f;
      float variation2 = 0.95f + ((i + 1) % 3) * 0.025f;
      
      // Ring 1 points
      float x1_r1 = cos(angle1) * radius1 * variation1;
      float z1_r1 = sin(angle1) * radius1 * variation1;
      float x2_r1 = cos(angle2) * radius1 * variation2;
      float z2_r1 = sin(angle2) * radius1 * variation2;
      
      // Ring 2 points
      float x1_r2 = cos(angle1) * radius2 * variation1;
      float z1_r2 = sin(angle1) * radius2 * variation1;
      float x2_r2 = cos(angle2) * radius2 * variation2;
      float z2_r2 = sin(angle2) * radius2 * variation2;
      
      // Connect rings with triangles
      if (radius1 > 0.01f && radius2 > 0.01f) {
        // Both rings have radius - make quads (2 triangles)
        glVertex3f(x1_r1, h1, z1_r1);
        glVertex3f(x2_r1, h1, z2_r1);
        glVertex3f(x1_r2, h2, z1_r2);
        
        glVertex3f(x2_r1, h1, z2_r1);
        glVertex3f(x2_r2, h2, z2_r2);
        glVertex3f(x1_r2, h2, z1_r2);
      } else if (radius1 > 0.01f) {
        // Bottom ring only - converge to point at top
        glVertex3f(x1_r1, h1, z1_r1);
        glVertex3f(x2_r1, h1, z2_r1);
        glVertex3f(0.0f, h2, 0.0f);
      } else if (radius2 > 0.01f) {
        // Top ring only - converge from point at bottom
        glVertex3f(0.0f, h1, 0.0f);
        glVertex3f(x1_r2, h2, z1_r2);
        glVertex3f(x2_r2, h2, z2_r2);
      }
    }
  }
  
  glEnd();
}

void drawTree(const Tree& tree) {
  glPushMatrix();
  glTranslated(tree.x, 0.0, tree.z);
  glScaled(tree.scale, tree.scale, tree.scale);
  
  // Draw main trunk - simple tapered cylinder
  glColor3f(tree.trunkR, tree.trunkG, tree.trunkB);
  glBegin(GL_QUADS);
  
  float trunkHeight = tree.height * 0.45f;
  float trunkBaseWidth = 0.5f;
  float trunkTopWidth = 0.35f;
  
  // 8 sides for trunk
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
    
    // Trunk face
    glVertex3f(x1_base, 0.0f, z1_base);
    glVertex3f(x2_base, 0.0f, z2_base);
    glVertex3f(x2_top, trunkHeight, z2_top);
    glVertex3f(x1_top, trunkHeight, z1_top);
  }
  
  glEnd();
  
  // Draw foliage as separate rounded clusters (eggs/balls) with trunk showing between
  float gap = 0.8f; // Gap between clusters to show trunk
  
  // Bottom cluster - large rounded ball
  float bottomBase = tree.height * 0.25f;
  float bottomTop = tree.height * 0.45f;
  glColor3f(tree.leavesR, tree.leavesG, tree.leavesB);
  drawRoundedCanopyCluster(bottomBase, bottomTop, 2.0f, 8);
  
  // Middle cluster - medium rounded ball (with gap above bottom)
  float middleBase = tree.height * 0.5f;
  float middleTop = tree.height * 0.68f;
  glColor3f(tree.leavesR * 0.95f, tree.leavesG * 0.95f, tree.leavesB * 0.95f);
  drawRoundedCanopyCluster(middleBase, middleTop, 1.4f, 8);
  
  // Top cluster - small rounded ball at top
  float topBase = tree.height * 0.73f;
  float topTop = tree.height * 0.95f;
  glColor3f(tree.leavesR * 0.9f, tree.leavesG * 0.9f, tree.leavesB * 0.9f);
  drawRoundedCanopyCluster(topBase, topTop, 0.9f, 6);
  
  // Add a few simple branch stubs
  glColor3f(tree.trunkR * 1.2f, tree.trunkG * 1.2f, tree.trunkB * 1.2f);
  glBegin(GL_TRIANGLES);
  
  float branchHeight = tree.height * 0.35f;
  
  // 3 branches at 120 degree intervals
  for (int i = 0; i < 3; i++) {
    float angle = (i / 3.0f) * 2.0f * M_PI;
    float bx = cos(angle) * 1.5f;
    float bz = sin(angle) * 1.5f;
    
    glVertex3f(0.0f, branchHeight + (i * 0.2f), 0.0f);
    glVertex3f(bx * 0.3f, branchHeight + (i * 0.2f) + 0.1f, bz * 0.3f);
    glVertex3f(bx, branchHeight + (i * 0.2f) - 0.1f, bz);
  }
  
  glEnd();
  
  glPopMatrix();
}

void drawBench(const Bench& bench) {
  glPushMatrix();
  glTranslated(bench.x, 0.0, bench.z);
  glRotated(bench.rotation, 0.0, 1.0, 0.0);
  
  // Dark weathered wood color
  glColor3f(0.18f, 0.16f, 0.15f);
  
  // Bench seat - fully enclosed with all 6 faces
  glBegin(GL_QUADS);
  
  // Top face
  glVertex3f(-1.0f, 0.5f, -0.3f);
  glVertex3f(1.0f, 0.5f, -0.3f);
  glVertex3f(1.0f, 0.5f, 0.3f);
  glVertex3f(-1.0f, 0.5f, 0.3f);
  
  // Bottom face
  glVertex3f(-1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, 0.3f);
  glVertex3f(-1.0f, 0.4f, 0.3f);
  
  // Front face
  glVertex3f(-1.0f, 0.4f, 0.3f);
  glVertex3f(1.0f, 0.4f, 0.3f);
  glVertex3f(1.0f, 0.5f, 0.3f);
  glVertex3f(-1.0f, 0.5f, 0.3f);
  
  // Back face
  glVertex3f(-1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.5f, -0.3f);
  glVertex3f(-1.0f, 0.5f, -0.3f);
  
  // Left side
  glVertex3f(-1.0f, 0.4f, -0.3f);
  glVertex3f(-1.0f, 0.4f, 0.3f);
  glVertex3f(-1.0f, 0.5f, 0.3f);
  glVertex3f(-1.0f, 0.5f, -0.3f);
  
  // Right side
  glVertex3f(1.0f, 0.4f, -0.3f);
  glVertex3f(1.0f, 0.4f, 0.3f);
  glVertex3f(1.0f, 0.5f, 0.3f);
  glVertex3f(1.0f, 0.5f, -0.3f);
  
  glEnd();
  
  // Bench back - fully enclosed
  glBegin(GL_QUADS);
  
  // Front face (facing player when sitting)
  glVertex3f(-1.0f, 0.5f, -0.35f);
  glVertex3f(1.0f, 0.5f, -0.35f);
  glVertex3f(1.0f, 1.2f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.35f);
  
  // Back face
  glVertex3f(-1.0f, 0.5f, -0.45f);
  glVertex3f(1.0f, 0.5f, -0.45f);
  glVertex3f(1.0f, 1.2f, -0.45f);
  glVertex3f(-1.0f, 1.2f, -0.45f);
  
  // Top edge
  glVertex3f(-1.0f, 1.2f, -0.45f);
  glVertex3f(1.0f, 1.2f, -0.45f);
  glVertex3f(1.0f, 1.2f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.35f);
  
  // Left side
  glVertex3f(-1.0f, 0.5f, -0.45f);
  glVertex3f(-1.0f, 0.5f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.35f);
  glVertex3f(-1.0f, 1.2f, -0.45f);
  
  // Right side
  glVertex3f(1.0f, 0.5f, -0.45f);
  glVertex3f(1.0f, 0.5f, -0.35f);
  glVertex3f(1.0f, 1.2f, -0.35f);
  glVertex3f(1.0f, 1.2f, -0.45f);
  
  glEnd();
  
  // Legs - fully enclosed rectangular posts (4 legs)
  glBegin(GL_QUADS);
  
  // LEFT FRONT LEG
  // Front face
  glVertex3f(-0.8f, 0.0f, 0.25f);
  glVertex3f(-0.7f, 0.0f, 0.25f);
  glVertex3f(-0.7f, 0.4f, 0.25f);
  glVertex3f(-0.8f, 0.4f, 0.25f);
  // Back face
  glVertex3f(-0.8f, 0.0f, 0.15f);
  glVertex3f(-0.7f, 0.0f, 0.15f);
  glVertex3f(-0.7f, 0.4f, 0.15f);
  glVertex3f(-0.8f, 0.4f, 0.15f);
  // Left side
  glVertex3f(-0.8f, 0.0f, 0.15f);
  glVertex3f(-0.8f, 0.0f, 0.25f);
  glVertex3f(-0.8f, 0.4f, 0.25f);
  glVertex3f(-0.8f, 0.4f, 0.15f);
  // Right side
  glVertex3f(-0.7f, 0.0f, 0.15f);
  glVertex3f(-0.7f, 0.0f, 0.25f);
  glVertex3f(-0.7f, 0.4f, 0.25f);
  glVertex3f(-0.7f, 0.4f, 0.15f);
  
  // RIGHT FRONT LEG
  // Front face
  glVertex3f(0.7f, 0.0f, 0.25f);
  glVertex3f(0.8f, 0.0f, 0.25f);
  glVertex3f(0.8f, 0.4f, 0.25f);
  glVertex3f(0.7f, 0.4f, 0.25f);
  // Back face
  glVertex3f(0.7f, 0.0f, 0.15f);
  glVertex3f(0.8f, 0.0f, 0.15f);
  glVertex3f(0.8f, 0.4f, 0.15f);
  glVertex3f(0.7f, 0.4f, 0.15f);
  // Left side
  glVertex3f(0.7f, 0.0f, 0.15f);
  glVertex3f(0.7f, 0.0f, 0.25f);
  glVertex3f(0.7f, 0.4f, 0.25f);
  glVertex3f(0.7f, 0.4f, 0.15f);
  // Right side
  glVertex3f(0.8f, 0.0f, 0.15f);
  glVertex3f(0.8f, 0.0f, 0.25f);
  glVertex3f(0.8f, 0.4f, 0.25f);
  glVertex3f(0.8f, 0.4f, 0.15f);
  
  // LEFT BACK LEG
  // Front face
  glVertex3f(-0.8f, 0.0f, -0.15f);
  glVertex3f(-0.7f, 0.0f, -0.15f);
  glVertex3f(-0.7f, 0.4f, -0.15f);
  glVertex3f(-0.8f, 0.4f, -0.15f);
  // Back face
  glVertex3f(-0.8f, 0.0f, -0.25f);
  glVertex3f(-0.7f, 0.0f, -0.25f);
  glVertex3f(-0.7f, 0.4f, -0.25f);
  glVertex3f(-0.8f, 0.4f, -0.25f);
  // Left side
  glVertex3f(-0.8f, 0.0f, -0.25f);
  glVertex3f(-0.8f, 0.0f, -0.15f);
  glVertex3f(-0.8f, 0.4f, -0.15f);
  glVertex3f(-0.8f, 0.4f, -0.25f);
  // Right side
  glVertex3f(-0.7f, 0.0f, -0.25f);
  glVertex3f(-0.7f, 0.0f, -0.15f);
  glVertex3f(-0.7f, 0.4f, -0.15f);
  glVertex3f(-0.7f, 0.4f, -0.25f);
  
  // RIGHT BACK LEG
  // Front face
  glVertex3f(0.7f, 0.0f, -0.15f);
  glVertex3f(0.8f, 0.0f, -0.15f);
  glVertex3f(0.8f, 0.4f, -0.15f);
  glVertex3f(0.7f, 0.4f, -0.15f);
  // Back face
  glVertex3f(0.7f, 0.0f, -0.25f);
  glVertex3f(0.8f, 0.0f, -0.25f);
  glVertex3f(0.8f, 0.4f, -0.25f);
  glVertex3f(0.7f, 0.4f, -0.25f);
  // Left side
  glVertex3f(0.7f, 0.0f, -0.25f);
  glVertex3f(0.7f, 0.0f, -0.15f);
  glVertex3f(0.7f, 0.4f, -0.15f);
  glVertex3f(0.7f, 0.4f, -0.25f);
  // Right side
  glVertex3f(0.8f, 0.0f, -0.25f);
  glVertex3f(0.8f, 0.0f, -0.15f);
  glVertex3f(0.8f, 0.4f, -0.15f);
  glVertex3f(0.8f, 0.4f, -0.25f);
  
  glEnd();
  
  glPopMatrix();
}
