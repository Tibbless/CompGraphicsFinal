#include "eerie_city.h"

void initializeBuildings() {
  buildings.clear();
  
  int blockSize = 25;
  int streetWidth = 8;
  int buildingSpacing = blockSize + streetWidth;
  
  for (int bx = -3; bx <= 3; bx++) {
    for (int bz = -3; bz <= 3; bz++) {
      // Skip center for player spawn
      if (abs(bx) <= 1 && abs(bz) <= 1) continue;
      
      double baseX = bx * buildingSpacing;
      double baseZ = bz * buildingSpacing;
      
      // Create 2-4 buildings per block
      int numBuildings = 2 + (rand() % 3);
      for (int i = 0; i < numBuildings; i++) {
        Building b;
        
        b.x = baseX + (rand() / double(RAND_MAX) - 0.5) * (blockSize - 10);
        b.z = baseZ + (rand() / double(RAND_MAX) - 0.5) * (blockSize - 10);
        
        b.width = 4.0 + (rand() / double(RAND_MAX)) * 6.0;
        b.depth = 4.0 + (rand() / double(RAND_MAX)) * 6.0;
        b.height = 8.0 + (rand() / double(RAND_MAX)) * 25.0;
        
        b.rotation = (rand() % 8) * 45.0;
        
        // Desaturated, dark colors
        float baseVal = 0.15f + (rand() / float(RAND_MAX)) * 0.15f;
        b.r = baseVal + (rand() / float(RAND_MAX)) * 0.05f;
        b.g = baseVal + (rand() / float(RAND_MAX)) * 0.05f;
        b.b = baseVal + (rand() / float(RAND_MAX)) * 0.08f;
        
        b.buildingType = rand() % 3;
        b.hasWindows = (rand() % 100) < 70;
        b.windowPattern = rand() % 4;
        
        buildings.push_back(b);
      }
    }
  }
  
  std::cout << "Generated " << buildings.size() << " buildings" << std::endl;
}

void initializeStreetLamps() {
  streetLamps.clear();
  
  int lampSpacing = 15;
  
  for (int x = -150; x <= 150; x += lampSpacing) {
    for (int z = -150; z <= 150; z += lampSpacing) {
      // Place lamps along streets
      if ((abs(x) % 33 < 4) || (abs(z) % 33 < 4)) {
        StreetLamp lamp;
        lamp.x = x + (rand() / double(RAND_MAX) - 0.5) * 2.0;
        lamp.z = z + (rand() / double(RAND_MAX) - 0.5) * 2.0;
        lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 2.0;
        lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
        lamp.isWorking = (rand() % 100) < 60; // 40% are broken
        
        streetLamps.push_back(lamp);
      }
    }
  }
  
  std::cout << "Generated " << streetLamps.size() << " street lamps" << std::endl;
}

void initializeAmbientObjects() {
  ambientObjects.clear();
  
  for (int i = 0; i < 200; i++) {
    double x = (rand() / double(RAND_MAX) - 0.5) * worldSize * 1.8;
    double z = (rand() / double(RAND_MAX) - 0.5) * worldSize * 1.8;
    
    // Don't place too close to buildings
    bool tooClose = false;
    for (const auto& building : buildings) {
      double dist = sqrt((x - building.x)*(x - building.x) + 
                        (z - building.z)*(z - building.z));
      if (dist < 8.0) {
        tooClose = true;
        break;
      }
    }
    
    if (!tooClose) {
      AmbientObject obj;
      obj.x = x;
      obj.z = z;
      obj.rotation = (rand() / double(RAND_MAX)) * 360.0;
      obj.objectType = rand() % 4;
      obj.scale = 0.3f + (rand() / float(RAND_MAX)) * 0.7f;
      
      ambientObjects.push_back(obj);
    }
  }
  
  std::cout << "Generated " << ambientObjects.size() << " ambient objects" << std::endl;
}