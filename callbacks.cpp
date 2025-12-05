#include "eerie_city.h"
#include <sstream>
#include <iomanip>

// ============================================================================
// DISPLAY CALLBACK
// ============================================================================

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  // Draw sky background first
  drawSky();
  
  // Setup first-person camera
  double lookX = playerX + sin(playerAngle * M_PI / 180.0) * cos(playerPitch * M_PI / 180.0);
  double lookY = playerY + sin(playerPitch * M_PI / 180.0);
  double lookZ = playerZ - cos(playerAngle * M_PI / 180.0) * cos(playerPitch * M_PI / 180.0);
  
  gluLookAt(playerX, playerY, playerZ,
            lookX, lookY, lookZ,
            0.0, 1.0, 0.0);
  
  // Update lighting after camera is set up
  // This ensures light positions are in the correct coordinate space
  updateLighting();
  setupStreetLampLights();
  
  // Draw world geometry
  drawGroundPlane();
  drawRoads();
  drawCityBlockSidewalks();
  
  // Draw all buildings
  for (const auto& building : buildings) {
    drawBuilding(building);
  }
  
  // Draw park elements
  for (const auto& tree : trees) {
    drawTree(tree);
  }
  
  for (const auto& bench : benches) {
    drawBench(bench);
  }
  
  // Draw industrial elements
  for (const auto& stack : smokestacks) {
    drawSmokestack(stack);
  }
  
  for (const auto& fence : fences) {
    drawFence(fence);
  }
  
  // Draw graveyard elements
  for (const auto& stone : gravestones) {
    drawGravestone(stone);
  }
  
  for (const auto& m : mausoleums) {
    drawMausoleum(m);
  }
  
  // Draw street lamps
  for (const auto& lamp : streetLamps) {
    drawStreetLamp(lamp);
  }
  
  // Note: Ambient objects disabled due to darkness/clutter with new lighting
  
  // Apply PS1-style visual effects
  applyDitherEffect();
  
  // ========================================
  // Draw HUD
  // ========================================
  
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  
  // Switch to 2D orthographic projection
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
  
  // Draw HUD text
  glColor3f(0.7f, 0.6f, 0.5f);
  
  glRasterPos2f(10, 20);
  Print("EERIE CITY - PS1 HORROR [BLOCK-BASED]");
  
  glRasterPos2f(10, 40);
  Print("Controls:");
  
  glRasterPos2f(10, 60);
  Print("Movement:");
  
  glRasterPos2f(10, 75);
  Print("  WASD / Arrow Keys - Move & Turn");
  
  glRasterPos2f(10, 90);
  Print("  Q/E - Strafe Left/Right");
  
  glRasterPos2f(10, 105);
  Print("  Z/X - Look Up/Down");
  
  glRasterPos2f(10, 125);
  Print("Atmosphere:");
  
  glRasterPos2f(10, 140);
  Print("  T - Toggle time auto-advance");
  
  glRasterPos2f(10, 155);
  Print("  N/M - Increase/Decrease noise");
  
  glRasterPos2f(10, 170);
  Print("  F/G - Increase/Decrease flicker");
  
  glRasterPos2f(10, 185);
  Print("  Shift+D - Toggle dither effect");
  
  glRasterPos2f(10, 205);
  Print("Other:");
  
  glRasterPos2f(10, 220);
  Print("  R - Reset position");
  
  glRasterPos2f(10, 235);
  Print("  ESC - Exit");
  
  // Display player stats
  glRasterPos2f(10, 260);
  std::ostringstream pos;
  pos << std::fixed << std::setprecision(1);
  pos << "Position: (" << playerX << ", " << playerZ << ")";
  Print(pos.str());
  
  glRasterPos2f(10, 275);
  std::ostringstream angle;
  angle << std::fixed << std::setprecision(0);
  angle << "Facing: " << playerAngle << " degrees";
  Print(angle.str());
  
  glRasterPos2f(10, 290);
  std::ostringstream time;
  time << std::fixed << std::setprecision(1);
  time << "Time: " << timeOfDay << ":00 (Night)";
  Print(time.str());
  
  glRasterPos2f(10, 305);
  std::ostringstream effects;
  effects << "Dither: " << (ditherEnabled ? "ON" : "OFF") 
          << " | Noise: " << (int)(noiseAmount * 100) << "%"
          << " | Flicker: " << (int)(flickerIntensity * 100) << "%";
  Print(effects.str());
  
  glRasterPos2f(10, 320);
  std::ostringstream objects;
  objects << "City: " << cityBlocks.size() << " blocks, "
          << buildings.size() << " buildings, " 
          << streetLamps.size() << " lamps, "
          << smokestacks.size() << " stacks, "
          << gravestones.size() << " graves";
  Print(objects.str());
  
  // Draw vignette effect for horror atmosphere
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Top edge gradient
  glBegin(GL_QUADS);
  glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
  glVertex2f(0, 0);
  glVertex2f(width, 0);
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glVertex2f(width, height * 0.2f);
  glVertex2f(0, height * 0.2f);
  glEnd();
  
  // Bottom edge gradient
  glBegin(GL_QUADS);
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glVertex2f(0, height * 0.8f);
  glVertex2f(width, height * 0.8f);
  glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
  glVertex2f(width, height);
  glVertex2f(0, height);
  glEnd();
  
  // Left edge gradient
  glBegin(GL_QUADS);
  glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
  glVertex2f(0, 0);
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glVertex2f(width * 0.15f, 0);
  glVertex2f(width * 0.15f, height);
  glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
  glVertex2f(0, height);
  glEnd();
  
  // Right edge gradient
  glBegin(GL_QUADS);
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glVertex2f(width * 0.85f, 0);
  glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
  glVertex2f(width, 0);
  glVertex2f(width, height);
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glVertex2f(width * 0.85f, height);
  glEnd();
  
  glDisable(GL_BLEND);
  
  // Restore 3D projection
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

// ============================================================================
// RESHAPE CALLBACK
// ============================================================================

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  double aspect = (height > 0) ? static_cast<double>(width) / height : 1.0;
  gluPerspective(fov, aspect, 0.1, 500.0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// ============================================================================
// KEYBOARD CALLBACK
// ============================================================================

void key(unsigned char ch, int /*x*/, int /*y*/) {
  switch(ch) {
    case 27: // ESC
      exit(0);
      break;
      
    // Reset player position
    case 'r':
    case 'R':
      playerX = 0.0;
      playerZ = 0.0;
      playerAngle = 0.0;
      playerPitch = 0.0;
      break;
      
    // Forward movement
    case 'w':
    case 'W':
      playerX += sin(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ -= cos(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    // Backward movement
    case 's':
    case 'S':
      playerX -= sin(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ += cos(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    // Turn left
    case 'a':
    case 'A':
      playerAngle -= turnSpeed;
      if (playerAngle < 0) playerAngle += 360.0;
      break;
      
    // Turn right or toggle dither (with shift)
    case 'd':
      playerAngle += turnSpeed;
      if (playerAngle >= 360) playerAngle -= 360.0;
      break;
      
    case 'D':
      ditherEnabled = !ditherEnabled;
      break;
      
    // Strafe left
    case 'q':
    case 'Q':
      playerX -= cos(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ -= sin(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    // Strafe right
    case 'e':
    case 'E':
      playerX += cos(playerAngle * M_PI / 180.0) * walkSpeed;
      playerZ += sin(playerAngle * M_PI / 180.0) * walkSpeed;
      break;
      
    // Look up
    case 'z':
    case 'Z':
      playerPitch += pitchSpeed;
      if (playerPitch > 89.0) playerPitch = 89.0;
      break;
      
    // Look down
    case 'x':
    case 'X':
      playerPitch -= pitchSpeed;
      if (playerPitch < -89.0) playerPitch = -89.0;
      break;
      
    // Toggle auto-time
    case 't':
    case 'T':
      autoTime = !autoTime;
      break;
      
    // Increase noise
    case 'n':
    case 'N':
      noiseAmount += 0.01;
      if (noiseAmount > 0.2) noiseAmount = 0.2;
      break;
      
    // Decrease noise
    case 'm':
    case 'M':
      noiseAmount -= 0.01;
      if (noiseAmount < 0.0) noiseAmount = 0.0;
      break;
      
    // Increase flicker
    case 'f':
    case 'F':
      flickerIntensity += 0.1;
      if (flickerIntensity > 2.0) flickerIntensity = 2.0;
      break;
      
    // Decrease flicker
    case 'g':
    case 'G':
      flickerIntensity -= 0.1;
      if (flickerIntensity < 0.0) flickerIntensity = 0.0;
      break;
      
    case 'v':
    case 'V':
      // Toggle vertical sync placeholder
      break;
  }
  
  glutPostRedisplay();
}

// ============================================================================
// SPECIAL KEY CALLBACK
// ============================================================================

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

// ============================================================================
// IDLE CALLBACK
// ============================================================================

void idle() {
  // Advance time if auto-time is enabled
  if (autoTime) {
    timeOfDay += daySpeed;
    if (timeOfDay >= 24.0) timeOfDay -= 24.0;
  }
  
  glutPostRedisplay();
}
