#include "eerie_city.h"

// Helper function to convert grid coordinates to world coordinates
void gridToWorld(int gridX, int gridZ, double& worldX, double& worldZ) {
  int totalBlockSize = blockSize + roadWidth;
  worldX = gridX * totalBlockSize;
  worldZ = gridZ * totalBlockSize;
}

// Generate a building block with multiple buildings
void generateBuildingBlock(CityBlock& block) {
  // Create 2-5 buildings per block
  int numBuildings = 2 + (rand() % 4);
  
  for (int i = 0; i < numBuildings; i++) {
    Building b;
    
    // Position within the block (leaving margins for streets)
    double marginX = blockSize * 0.15;
    double marginZ = blockSize * 0.15;
    double usableWidth = blockSize - (2 * marginX);
    double usableDepth = blockSize - (2 * marginZ);
    
    b.x = block.worldX + marginX + (rand() / double(RAND_MAX)) * usableWidth;
    b.z = block.worldZ + marginZ + (rand() / double(RAND_MAX)) * usableDepth;
    
    // Building dimensions
    b.width = 3.0 + (rand() / double(RAND_MAX)) * 5.0;
    b.depth = 3.0 + (rand() / double(RAND_MAX)) * 5.0;
    b.height = 8.0 + (rand() / double(RAND_MAX)) * 28.0;
    
    // Rotation - align mostly to grid with some variation
    b.rotation = (rand() % 4) * 90.0 + (rand() / double(RAND_MAX) - 0.5) * 10.0;
    
    // Desaturated, dark colors for PS1 horror aesthetic
    float baseVal = 0.15f + (rand() / float(RAND_MAX)) * 0.15f;
    b.r = baseVal + (rand() / float(RAND_MAX)) * 0.05f;
    b.g = baseVal + (rand() / float(RAND_MAX)) * 0.05f;
    b.b = baseVal + (rand() / float(RAND_MAX)) * 0.08f;
    
    b.buildingType = rand() % 3;
    b.hasWindows = (rand() % 100) < 75;
    b.windowPattern = rand() % 4;
    
    // Add building to global list and track index in block
    block.buildingIndices.push_back(buildings.size());
    buildings.push_back(b);
  }
}

// Generate a park block with trees and benches
void generateParkBlock(CityBlock& block) {
  // Generate 8-15 trees scattered through the park
  int numTrees = 8 + (rand() % 8);
  
  for (int i = 0; i < numTrees; i++) {
    Tree tree;
    
    // Position within block with margins
    double marginX = blockSize * 0.2;
    double marginZ = blockSize * 0.2;
    double usableWidth = blockSize - (2 * marginX);
    double usableDepth = blockSize - (2 * marginZ);
    
    tree.x = block.worldX + marginX + (rand() / double(RAND_MAX)) * usableWidth;
    tree.z = block.worldZ + marginZ + (rand() / double(RAND_MAX)) * usableDepth;
    
    // Tree properties
    tree.height = 4.0 + (rand() / double(RAND_MAX)) * 5.0;
    tree.scale = 0.7f + (rand() / float(RAND_MAX)) * 0.6f;
    
    // Very dark, dead-looking trees for horror aesthetic
    tree.trunkR = 0.12f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.trunkG = 0.10f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.trunkB = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
    
    // Dark, sickly foliage
    tree.leavesR = 0.08f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.leavesG = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
    tree.leavesB = 0.06f + (rand() / float(RAND_MAX)) * 0.04f;
    
    trees.push_back(tree);
  }
  
  // Generate 3-6 benches
  int numBenches = 3 + (rand() % 4);
  
  for (int i = 0; i < numBenches; i++) {
    Bench bench;
    
    double marginX = blockSize * 0.25;
    double marginZ = blockSize * 0.25;
    double usableWidth = blockSize - (2 * marginX);
    double usableDepth = blockSize - (2 * marginZ);
    
    bench.x = block.worldX + marginX + (rand() / double(RAND_MAX)) * usableWidth;
    bench.z = block.worldZ + marginZ + (rand() / double(RAND_MAX)) * usableDepth;
    bench.rotation = (rand() % 4) * 90.0;
    
    benches.push_back(bench);
  }
}


// Initialize the city grid with blocks
void initializeCityGrid() {
  cityBlocks.clear();
  buildings.clear();
  trees.clear();
  benches.clear();
  
  int halfGrid = cityGridSize / 2;
  
  for (int gx = -halfGrid; gx <= halfGrid; gx++) {
    for (int gz = -halfGrid; gz <= halfGrid; gz++) {
      CityBlock block;
      block.gridX = gx;
      block.gridZ = gz;
      
      gridToWorld(gx, gz, block.worldX, block.worldZ);
      
      // Skip center blocks for player spawn area
      if (abs(gx) <= 1 && abs(gz) <= 1) {
        block.type = BLOCK_EMPTY;
      } 
      // Add variety: 70% buildings, 30% parks
      else if ((rand() % 100) < 70) {
        block.type = BLOCK_BUILDING;
        generateBuildingBlock(block);
      } 
      else {
        block.type = BLOCK_PARK;
        generateParkBlock(block);
      }
      
      cityBlocks.push_back(block);
    }
  }
  
  std::cout << "Generated " << cityBlocks.size() << " city blocks" << std::endl;
  std::cout << "Total buildings: " << buildings.size() << std::endl;
  std::cout << "Total trees: " << trees.size() << std::endl;
  std::cout << "Total benches: " << benches.size() << std::endl;
}

// Generate street lamps along roads between blocks
void generateRoadLights() {
  streetLamps.clear();
  
  int halfGrid = cityGridSize / 2;
  int totalBlockSize = blockSize + roadWidth;
  int lampSpacing = 12;
  
  // Generate lamps along horizontal roads
  for (int gx = -halfGrid; gx <= halfGrid + 1; gx++) {
    double roadCenterX = gx * totalBlockSize - roadWidth / 2.0;
    
    for (int gz = -halfGrid; gz <= halfGrid; gz++) {
      double blockWorldZ = gz * totalBlockSize;
      
      // Place lamps along the length of this road segment
      for (int offset = 0; offset < blockSize; offset += lampSpacing) {
        // Lamps on both sides of the road
        for (int side = -1; side <= 1; side += 2) {
          StreetLamp lamp;
          lamp.x = roadCenterX + side * (roadWidth / 2.5);
          lamp.z = blockWorldZ + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
          lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
          lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
          lamp.isWorking = (rand() % 100) < 65; // 35% are broken for atmosphere
          
          streetLamps.push_back(lamp);
        }
      }
    }
  }
  
  // Generate lamps along vertical roads
  for (int gz = -halfGrid; gz <= halfGrid + 1; gz++) {
    double roadCenterZ = gz * totalBlockSize - roadWidth / 2.0;
    
    for (int gx = -halfGrid; gx <= halfGrid; gx++) {
      double blockWorldX = gx * totalBlockSize;
      
      // Place lamps along the length of this road segment
      for (int offset = 0; offset < blockSize; offset += lampSpacing) {
        // Lamps on both sides of the road
        for (int side = -1; side <= 1; side += 2) {
          StreetLamp lamp;
          lamp.x = blockWorldX + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
          lamp.z = roadCenterZ + side * (roadWidth / 2.5);
          lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
          lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
          lamp.isWorking = (rand() % 100) < 65;
          
          streetLamps.push_back(lamp);
        }
      }
    }
  }
  
  std::cout << "Generated " << streetLamps.size() << " street lamps along roads" << std::endl;
}

// Initialize ambient objects (trash, debris, etc.)
void initializeAmbientObjects() {
  ambientObjects.clear();
  
  // Place objects on streets and in blocks
  for (int i = 0; i < 300; i++) {
    double x = (rand() / double(RAND_MAX) - 0.5) * worldSize * 1.5;
    double z = (rand() / double(RAND_MAX) - 0.5) * worldSize * 1.5;
    
    // Don't place too close to buildings
    bool tooClose = false;
    for (const auto& building : buildings) {
      double dist = sqrt((x - building.x)*(x - building.x) + 
                        (z - building.z)*(z - building.z));
      if (dist < 6.0) {
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
