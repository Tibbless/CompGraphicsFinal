#include "eerie_city.h"
#include <sstream>
#include <iomanip>

void display() {
  drawSky();
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  // First-person camera
  double lookX = playerX + sin(playerAngle * M_PI / 180.0) * cos(playerPitch * M_PI / 180.0);
  double lookY = playerY + sin(playerPitch * M_PI / 180.0);
  double lookZ = playerZ - cos(playerAngle * M_PI / 180.0) * cos(playerPitch * M_PI / 180.0);
  
  gluLookAt(playerX, playerY, playerZ,
            lookX, lookY, lookZ,
            0.0, 1.0, 0.0);
  
  updateLighting();
  
  // Draw world
  drawGroundPlane();
  
  for (const auto& building : buildings) {
    drawBuilding(building);
  }
  
  for (const auto& lamp : streetLamps) {
    drawStreetLamp(lamp);
  }
  
  for (const auto& obj : ambientObjects) {
    drawAmbientObject(obj);
  }
  
  // Apply PS1 effects
  applyDitherEffect();
  
  // Draw HUD
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  int width = viewport[2];
  int height = viewport[3];
  
  glOrtho(0, width, height, 0, -1, 1);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  glColor3f(0.7f, 0.6f, 0.5f);
  
  glRasterPos2f(10, 20);
  Print("EERIE CITY - PS1 HORROR AESTHETIC");
  
  glRasterPos2f(10, 40);
  Print("Controls:");
  
  glRasterPos2f(10, 55);
  Print("WASD / Arrow Keys - Move");
  
  glRasterPos2f(10, 70);
  Print("Q/E - Strafe");
  
  glRasterPos2f(10, 85);
  Print("Z/X - Look Up/Down");
  
  glRasterPos2f(10, 100);
  Print("T - Toggle time auto-advance");
  
  glRasterPos2f(10, 115);
  Print("N - Increase noise/dither");
  
  glRasterPos2f(10, 130);
  Print("M - Decrease noise/dither");
  
  glRasterPos2f(10, 145);
  Print("F - Increase flicker");
  
  glRasterPos2f(10, 160);
  Print("G - Decrease flicker");
  
  glRasterPos2f(10, 175);
  Print("D - Toggle dither");
  
  glRasterPos2f(10, 190);
  Print("R - Reset position");
  
  glRasterPos2f(10, 205);
  Print("ESC - Exit");
  
  glRasterPos2f(10, 230);
  std::ostringstream pos;
  pos << std::fixed << std::setprecision(1);
  pos << "Position: (" << playerX << ", " << playerZ << ")";
  Print(pos.str());
  
  glRasterPos2f(10, 245);
  std::ostringstream angle;
  angle << std::fixed << std::setprecision(0);
  angle << "Facing: " << playerAngle << " degrees";
  Print(angle.str());
  
  glRasterPos2f(10, 260);
  std::ostringstream time;
  time << std::fixed << std::setprecision(1);
  time << "Time: " << timeOfDay << ":00 (Night)";
  Print(time.str());
  
  glRasterPos2f(10, 275);
  std::ostringstream effects;
  effects << "Dither: " << (ditherEnabled ? "ON" : "OFF") 
          << " | Noise: " << (int)(noiseAmount * 100) << "%"
          << " | Flicker: " << (int)(flickerIntensity * 100) << "%";
  Print(effects.str());
  
  glRasterPos2f(10, 290);
  std::ostringstream objects;
  objects << "Scene: " << buildings.size() << " buildings, " 
          << streetLamps.size() << " lamps, "
          << ambientObjects.size() << " objects";
  Print(objects.str());
  
  // Vignette effect
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  float vignetteSize = 0.7f;
  glBegin(GL_QUADS);
  
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glVertex2f(width * (1.0f - vignetteSize) / 2.0f, height * (1.0f - vignetteSize) / 2.0f);
  glVertex2f(width * (1.0f + vignetteSize) / 2.0f, height * (1.0f - vignetteSize) / 2.0f);
  
  glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
  glVertex2f(width * (1.0f + vignetteSize) / 2.0f, height * (1.0f + vignetteSize) / 2.0f);
  glVertex2f(width * (1.0f - vignetteSize) / 2.0f, height * (1.0f + vignetteSize) / 2.0f);
  
  glEnd();
  
  glBegin(GL_QUADS);
  glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
  glVertex2f(0, 0);
  glVertex2f(width, 0);
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glVertex2f(width, height * 0.15f);
  glVertex2f(0, height * 0.15f);
  glEnd();
  
  glBegin(GL_QUADS);
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glVertex2f(0, height * 0.85f);
  glVertex2f(width, height * 0.85f);
  glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
  glVertex2f(width, height);
  glVertex2f(0, height);
  glEnd();
  
  glDisable(GL_BLEND);
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  
  ErrCheck("display");
  glFlush();
  glutSwapBuffers();
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  double aspect = (height > 0) ? static_cast<double>(width) / height : 1.0;
  gluPerspective(fov, aspect, 0.1, 500.0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void key(unsigned char ch, int /*x*/, int /*y*/) {
  switch(ch) {
    case 27: // ESC
      exit(0);
      break;
      
    case 'r':
    case 'R':
      playerX = 0.0;
      playerZ = 0.0;
      playerAngle = 0.0;
      playerPitch = 0.0;
      break;
      
    case 'w':
    case 'W':
      playerX += sin(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ -= cos(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    case 's':
    case 'S':
      playerX -= sin(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ += cos(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    case 'a':
    case 'A':
      playerAngle -= turnSpeed;
      if (playerAngle < 0) playerAngle += 360.0;
      break;
      
    case 'd':
    case 'D':
      if (ch == 'd' || ch == 'D') {
        playerAngle += turnSpeed;
        if (playerAngle >= 360) playerAngle -= 360.0;
      }
      break;
      
    case 'q':
    case 'Q':
      playerX -= cos(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ -= sin(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    case 'e':
    case 'E':
      playerX += cos(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ += sin(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    case 'z':
    case 'Z':
      playerPitch += pitchSpeed;
      if (playerPitch > 89.0) playerPitch = 89.0;
      break;
      
    case 'x':
    case 'X':
      playerPitch -= pitchSpeed;
      if (playerPitch < -89.0) playerPitch = -89.0;
      break;
      
    case 't':
    case 'T':
      autoTime = !autoTime;
      break;
      
    case 'n':
    case 'N':
      noiseAmount += 0.01;
      if (noiseAmount > 0.2) noiseAmount = 0.2;
      break;
      
    case 'm':
    case 'M':
      noiseAmount -= 0.01;
      if (noiseAmount < 0.0) noiseAmount = 0.0;
      break;
      
    case 'f':
    case 'F':
      flickerIntensity += 0.1;
      if (flickerIntensity > 2.0) flickerIntensity = 2.0;
      break;
      
    case 'g':
    case 'G':
      flickerIntensity -= 0.1;
      if (flickerIntensity < 0.0) flickerIntensity = 0.0;
      break;
  }
  
  // Special handling for 'D' key to toggle dither
  if (ch == 'D') {
    ditherEnabled = !ditherEnabled;
  }
  
  glutPostRedisplay();
}

void special(int key, int /*x*/, int /*y*/) {
  switch(key) {
    case GLUT_KEY_UP:
      playerX += sin(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ -= cos(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    case GLUT_KEY_DOWN:
      playerX -= sin(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ += cos(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    case GLUT_KEY_LEFT:
      playerAngle -= turnSpeed;
      if (playerAngle < 0) playerAngle += 360.0;
      break;
      
    case GLUT_KEY_RIGHT:
      playerAngle += turnSpeed;
      if (playerAngle >= 360) playerAngle -= 360.0;
      break;
  }
  
  glutPostRedisplay();
}

void idle() {
  if (autoTime) {
    timeOfDay += daySpeed;
    if (timeOfDay >= 24.0) timeOfDay -= 24.0;
  }
  
  glutPostRedisplay();
}