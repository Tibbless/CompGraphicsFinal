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

// Generate a park block with trees, benches, and atmospheric lighting
void generateParkBlock(CityBlock& block) {
  // Park layout: create a more organized feeling with clusters and pathways
  
  // Define park areas - center open space with tree clusters around edges
  double parkCenterX = block.worldX + blockSize / 2.0;
  double parkCenterZ = block.worldZ + blockSize / 2.0;
  
  // Generate tree clusters pushed to the edges, leaving center clear
  int numTreeClusters = 4; // Always 4 clusters - one near each corner/edge
  
  for (int cluster = 0; cluster < numTreeClusters; cluster++) {
    // Force clusters to specific edge/corner positions
    double clusterX, clusterZ;
    double edgeDistance = blockSize * 0.35; // Distance from center to cluster
    
    switch(cluster) {
      case 0: // North-East corner area
        clusterX = parkCenterX + edgeDistance;
        clusterZ = parkCenterZ - edgeDistance;
        break;
      case 1: // South-East corner area
        clusterX = parkCenterX + edgeDistance;
        clusterZ = parkCenterZ + edgeDistance;
        break;
      case 2: // South-West corner area
        clusterX = parkCenterX - edgeDistance;
        clusterZ = parkCenterZ + edgeDistance;
        break;
      case 3: // North-West corner area
        clusterX = parkCenterX - edgeDistance;
        clusterZ = parkCenterZ - edgeDistance;
        break;
      default:
        clusterX = parkCenterX;
        clusterZ = parkCenterZ;
        break;
    }
    
    // ONE larger, more detailed tree per cluster
    Tree tree;
    
    // Position at cluster center with slight variation
    tree.x = clusterX + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    tree.z = clusterZ + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    
    // Ensure tree stays within park bounds with margin
    double margin = 4.0;
    if (tree.x < block.worldX + margin) tree.x = block.worldX + margin;
    if (tree.x > block.worldX + blockSize - margin) tree.x = block.worldX + blockSize - margin;
    if (tree.z < block.worldZ + margin) tree.z = block.worldZ + margin;
    if (tree.z > block.worldZ + blockSize - margin) tree.z = block.worldZ + blockSize - margin;
    
    // Tree properties - larger and more prominent as single specimens
    tree.height = 6.0 + (rand() / double(RAND_MAX)) * 6.0;  // Taller: 6-12 units
    tree.scale = 1.2f + (rand() / float(RAND_MAX)) * 0.8f;  // Bigger: 1.2-2.0 scale
    
    // Very dark, dead-looking trees for horror aesthetic
    tree.trunkR = 0.12f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.trunkG = 0.10f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.trunkB = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
    
    // Dark, sickly foliage
    tree.leavesR = 0.08f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.leavesG = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
    tree.leavesB = 0.06f + (rand() / float(RAND_MAX)) * 0.04f;
    
    // ASSIGN RANDOM TREE TYPE!
    // Distribution: 30% layered, 45% dead, 25% twisted
    int typeRoll = rand() % 100;
    if (typeRoll < 30) {
      tree.type = TREE_LAYERED;
    } else if (typeRoll < 75) {
      tree.type = TREE_DEAD;  // Most common for horror aesthetic
    } else {
      tree.type = TREE_TWISTED;
    }
    
    trees.push_back(tree);
  }
  
  // Generate benches very close to edges, facing center of park
  int numBenches = 6 + (rand() % 3); // 6-8 benches per park for perimeter coverage
  
  for (int i = 0; i < numBenches; i++) {
    Bench bench;
    
    // Place benches very close to the edges
    int placement = rand() % 4;
    double closeMargin = blockSize * 0.12; // Very close to edge
    double alongEdge = blockSize * 0.2 + (rand() / double(RAND_MAX)) * (blockSize * 0.6);
    
    switch(placement) {
      case 0: // North edge, facing south into park
        bench.x = block.worldX + alongEdge;
        bench.z = block.worldZ + closeMargin;
        bench.rotation = 0.0;
        break;
      case 1: // South edge, facing north into park
        bench.x = block.worldX + alongEdge;
        bench.z = block.worldZ + blockSize - closeMargin;
        bench.rotation = 180.0;
        break;
      case 2: // East edge, facing west into park
        bench.x = block.worldX + blockSize - closeMargin;
        bench.z = block.worldZ + alongEdge;
        bench.rotation = 270.0;
        break;
      case 3: // West edge, facing east into park
        bench.x = block.worldX + closeMargin;
        bench.z = block.worldZ + alongEdge;
        bench.rotation = 90.0;
        break;
    }
    
    benches.push_back(bench);
  }
  
  // Add atmospheric park lighting - ring around the center area
  int numParkLights = 6; // Fixed number for consistent ring pattern
  
  for (int i = 0; i < numParkLights; i++) {
    StreetLamp lamp;
    
    // Position lights in a ring around the center (hexagonal-ish pattern)
    double angle = (i / (double)numParkLights) * 2.0 * M_PI;
    double ringRadius = blockSize * 0.28; // Distance from center
    
    lamp.x = parkCenterX + cos(angle) * ringRadius;
    lamp.z = parkCenterZ + sin(angle) * ringRadius;
    
    // Add small random offset for less perfect positioning
    lamp.x += (rand() / double(RAND_MAX) - 0.5) * 2.0;
    lamp.z += (rand() / double(RAND_MAX) - 0.5) * 2.0;
    
    // Shorter, more atmospheric park lights
    lamp.height = 3.0 + (rand() / double(RAND_MAX)) * 1.0;
    lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
    
    // Higher chance of working lights in parks (safer feeling)
    lamp.isWorking = (rand() % 100) < 75; // 75% working (vs 65% on streets)
    
    // Track this lamp in the block
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
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
  // Note: Park lights are now generated within generateParkBlock()
  // This function only handles street/road lighting
  
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
          lamp.isWorking = (rand() % 100) < 65; // 65% are working, 35% broken for atmosphere
          
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
  
  std::cout << "Generated " << streetLamps.size() << " total lamps (streets + parks)" << std::endl;
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
