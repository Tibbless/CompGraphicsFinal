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
  
  // Windows with eerie glow
  if (building.hasWindows) {
    int windowsPerFloor = 2 + (building.windowPattern % 3);
    int numFloors = (int)(building.height / 3.0);
    
    for (int floor = 1; floor < numFloors; floor++) {
      for (int w = 0; w < windowsPerFloor; w++) {
        double windowY = floor * 3.0;
        double windowX = -building.width * 0.8 + (w * building.width * 1.6 / windowsPerFloor);
        
        // Random chance of lit window
        if ((rand() % 100) < 30) {
          float glowR = 0.8f + (rand() / float(RAND_MAX)) * 0.2f;
          float glowG = 0.6f + (rand() / float(RAND_MAX)) * 0.2f;
          float glowB = 0.3f;
          
          glColor3f(glowR, glowG, glowB);
        } else {
          glColor3f(0.1f, 0.1f, 0.15f);
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
  
  // Lamp post
  glColor3f(0.2f, 0.2f, 0.25f);
  glBegin(GL_QUADS);
  
  glVertex3f(-0.15f, 0.0f, -0.15f);
  glVertex3f(0.15f, 0.0f, -0.15f);
  glVertex3f(0.15f, lamp.height, -0.15f);
  glVertex3f(-0.15f, lamp.height, -0.15f);
  
  glVertex3f(-0.15f, 0.0f, 0.15f);
  glVertex3f(0.15f, 0.0f, 0.15f);
  glVertex3f(0.15f, lamp.height, 0.15f);
  glVertex3f(-0.15f, lamp.height, 0.15f);
  
  glEnd();
  
  // Lamp head
  if (lamp.isWorking) {
    float flicker = 0.7f + sin(timeOfDay * 10.0 + lamp.flickerPhase) * 0.3f * flickerIntensity;
    flicker = fmax(0.3f, fmin(1.0f, flicker));
    
    glDisable(GL_LIGHTING);
    glColor3f(0.9f * flicker, 0.7f * flicker, 0.4f * flicker);
    
    glBegin(GL_QUADS);
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height + 0.8f, -0.4f);
    glVertex3f(-0.4f, lamp.height + 0.8f, -0.4f);
    glEnd();
    
    glEnable(GL_LIGHTING);
  } else {
    // Broken lamp
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(-0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height, -0.4f);
    glVertex3f(0.4f, lamp.height + 0.8f, -0.4f);
    glVertex3f(-0.4f, lamp.height + 0.8f, -0.4f);
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
  
  // Street lines with imperfections
  glDisable(GL_LIGHTING);
  glColor3f(0.2f, 0.2f, 0.22f);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  
  int streetSpacing = 33;
  for (int x = -200; x <= 200; x += streetSpacing) {
    float offset = sin(x * 0.1f) * 0.5f;
    glVertex3d(x + offset, 0.01, -worldSize);
    glVertex3d(x + offset, 0.01, worldSize);
  }
  
  for (int z = -200; z <= 200; z += streetSpacing) {
    float offset = sin(z * 0.1f) * 0.5f;
    glVertex3d(-worldSize, 0.01, z + offset);
    glVertex3d(worldSize, 0.01, z + offset);
  }
  
  glEnd();
  glEnable(GL_LIGHTING);
}

void drawSky() {
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  // Dark gradient sky
  glBegin(GL_QUADS);
  
  glColor3f(0.02f, 0.02f, 0.05f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  
  glColor3f(0.08f, 0.06f, 0.12f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  
  glEnd();
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
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