#include "eerie_city.h"

// ============================================================================
// GRID COORDINATE CONVERSION
// ============================================================================

// Convert grid coordinates to world coordinates
void gridToWorld(int gridX, int gridZ, double& worldX, double& worldZ) {
  int totalBlockSize = blockSize + roadWidth;
  worldX = gridX * totalBlockSize;
  worldZ = gridZ * totalBlockSize;
}

// Helper function to add sidewalk lamps around a block perimeter
void addSidewalkLamps(CityBlock& block) {
  float sidewalkWidth = 2.0f;
  int lampSpacing = 12;
  
  // North edge
  for (int offset = 6; offset < blockSize - 6; offset += lampSpacing) {
    StreetLamp lamp;
    lamp.x = block.worldX + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    lamp.z = block.worldZ + sidewalkWidth * 0.8;
    lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
    lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
    lamp.isWorking = (rand() % 100) < 65;
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
  }
  
  // South edge
  for (int offset = 6; offset < blockSize - 6; offset += lampSpacing) {
    StreetLamp lamp;
    lamp.x = block.worldX + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    lamp.z = block.worldZ + blockSize - sidewalkWidth * 0.8;
    lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
    lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
    lamp.isWorking = (rand() % 100) < 65;
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
  }
  
  // West edge
  for (int offset = 6; offset < blockSize - 6; offset += lampSpacing) {
    StreetLamp lamp;
    lamp.x = block.worldX + sidewalkWidth * 0.8;
    lamp.z = block.worldZ + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
    lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
    lamp.isWorking = (rand() % 100) < 65;
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
  }
  
  // East edge
  for (int offset = 6; offset < blockSize - 6; offset += lampSpacing) {
    StreetLamp lamp;
    lamp.x = block.worldX + blockSize - sidewalkWidth * 0.8;
    lamp.z = block.worldZ + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
    lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
    lamp.isWorking = (rand() % 100) < 65;
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
  }
}

// ============================================================================
// BUILDING BLOCK GENERATION
// ============================================================================

void generateBuildingBlock(CityBlock& block) {
  // Create 3-6 buildings per block
  int numBuildings = 3 + (rand() % 4);
  
  std::vector<Building> candidates;
  int maxAttempts = 50;
  
  for (int i = 0; i < numBuildings; i++) {
    bool placed = false;
    int attempts = 0;
    
    while (!placed && attempts < maxAttempts) {
      attempts++;
      
      Building b;
      
      // Building dimensions (smaller for more density)
      b.width = 2.0 + (rand() / double(RAND_MAX)) * 3.0;
      b.depth = 2.0 + (rand() / double(RAND_MAX)) * 3.0;
      b.height = 8.0 + (rand() / double(RAND_MAX)) * 28.0;
      
      // Rotation with slight variation
      b.rotation = (rand() % 4) * 90.0 + (rand() / double(RAND_MAX) - 0.5) * 10.0;
      
      // Calculate maximum dimension accounting for rotation
      double maxDim = fmax(b.width, b.depth);
      
      // Position within block with margins
      double marginX = blockSize * 0.15 + maxDim;
      double marginZ = blockSize * 0.15 + maxDim;
      double usableWidth = blockSize - (2 * marginX);
      double usableDepth = blockSize - (2 * marginZ);
      
      // Skip if building too large for block
      if (usableWidth <= 0 || usableDepth <= 0) {
        break;
      }
      
      b.x = block.worldX + marginX + (rand() / double(RAND_MAX)) * usableWidth;
      b.z = block.worldZ + marginZ + (rand() / double(RAND_MAX)) * usableDepth;
      
      // Desaturated, dark PS1 horror colors
      float baseVal = 0.15f + (rand() / float(RAND_MAX)) * 0.15f;
      b.r = baseVal + (rand() / float(RAND_MAX)) * 0.05f;
      b.g = baseVal + (rand() / float(RAND_MAX)) * 0.05f;
      b.b = baseVal + (rand() / float(RAND_MAX)) * 0.08f;
      
      b.buildingType = rand() % 3;
      b.hasWindows = (rand() % 100) < 95;  // Almost all buildings have windows
      b.windowPattern = rand() % 4;
      
      // Check block boundaries
      bool outsideBlock = false;
      if (b.x - maxDim < block.worldX ||
          b.x + maxDim > block.worldX + blockSize ||
          b.z - maxDim < block.worldZ ||
          b.z + maxDim > block.worldZ + blockSize) {
        outsideBlock = true;
      }
      
      if (outsideBlock) {
        continue;
      }
      
      // Check for overlaps with existing buildings
      bool overlaps = false;
      double minSeparation = 1.0;
      
      for (const auto& existing : candidates) {
        double dx = b.x - existing.x;
        double dz = b.z - existing.z;
        double distance = sqrt(dx*dx + dz*dz);
        
        double maxDim1 = fmax(b.width, b.depth);
        double maxDim2 = fmax(existing.width, existing.depth);
        double minDistance = maxDim1 + maxDim2 + minSeparation;
        
        if (distance < minDistance) {
          overlaps = true;
          break;
        }
      }
      
      if (!overlaps) {
        candidates.push_back(b);
        placed = true;
      }
    }
  }
  
  // Add successfully placed buildings to global list
  for (const auto& building : candidates) {
    block.buildingIndices.push_back(buildings.size());
    buildings.push_back(building);
  }
  
  // Generate street lamps on sidewalks and trees inside the block
  float sidewalkWidth = 2.0f;
  float innerMargin = 4.0f;  // Distance from sidewalk into the block for trees
  int itemSpacing = 12;
  float minTreeDistance = 4.0f;  // Minimum distance from trees to buildings
  
  // North edge (top)
  for (int offset = 6; offset < blockSize - 6; offset += itemSpacing) {
    double posX = block.worldX + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    
    // 50% chance: lamp on sidewalk OR tree inside block
    if (rand() % 2 == 0) {
      // Street lamp on sidewalk
      StreetLamp lamp;
      lamp.x = posX;
      lamp.z = block.worldZ + sidewalkWidth * 0.8;  // On sidewalk
      lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
      lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
      lamp.isWorking = (rand() % 100) < 65;
      block.lampIndices.push_back(streetLamps.size());
      streetLamps.push_back(lamp);
    } else {
      // Tree inside block - check for collision with buildings
      double treeX = posX;
      double treeZ = block.worldZ + sidewalkWidth + innerMargin;
      
      bool tooCloseToBuilding = false;
      for (const auto& building : candidates) {
        double dx = treeX - building.x;
        double dz = treeZ - building.z;
        double distance = sqrt(dx*dx + dz*dz);
        double maxDim = fmax(building.width, building.depth);
        
        if (distance < maxDim + minTreeDistance) {
          tooCloseToBuilding = true;
          break;
        }
      }
      
      if (!tooCloseToBuilding) {
        Tree tree;
        tree.x = treeX;
        tree.z = treeZ;
        tree.height = 6.0 + (rand() / double(RAND_MAX)) * 6.0;
        tree.scale = 1.0f + (rand() / float(RAND_MAX)) * 0.5f;
        tree.trunkR = 0.12f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.trunkG = 0.10f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.trunkB = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
        tree.leavesR = 0.08f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.leavesG = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
        tree.leavesB = 0.06f + (rand() / float(RAND_MAX)) * 0.04f;
        int treeTypeRoll = rand() % 3;
        tree.type = (treeTypeRoll == 0) ? TREE_LAYERED : (treeTypeRoll == 1) ? TREE_DEAD : TREE_TWISTED;
        trees.push_back(tree);
      }
    }
  }
  
  // South edge (bottom)
  for (int offset = 6; offset < blockSize - 6; offset += itemSpacing) {
    double posX = block.worldX + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    
    if (rand() % 2 == 0) {
      // Street lamp on sidewalk
      StreetLamp lamp;
      lamp.x = posX;
      lamp.z = block.worldZ + blockSize - sidewalkWidth * 0.8;  // On sidewalk
      lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
      lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
      lamp.isWorking = (rand() % 100) < 65;
      block.lampIndices.push_back(streetLamps.size());
      streetLamps.push_back(lamp);
    } else {
      // Tree inside block - check for collision with buildings
      double treeX = posX;
      double treeZ = block.worldZ + blockSize - sidewalkWidth - innerMargin;
      
      bool tooCloseToBuilding = false;
      for (const auto& building : candidates) {
        double dx = treeX - building.x;
        double dz = treeZ - building.z;
        double distance = sqrt(dx*dx + dz*dz);
        double maxDim = fmax(building.width, building.depth);
        
        if (distance < maxDim + minTreeDistance) {
          tooCloseToBuilding = true;
          break;
        }
      }
      
      if (!tooCloseToBuilding) {
        Tree tree;
        tree.x = treeX;
        tree.z = treeZ;
        tree.height = 6.0 + (rand() / double(RAND_MAX)) * 6.0;
        tree.scale = 1.0f + (rand() / float(RAND_MAX)) * 0.5f;
        tree.trunkR = 0.12f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.trunkG = 0.10f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.trunkB = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
        tree.leavesR = 0.08f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.leavesG = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
        tree.leavesB = 0.06f + (rand() / float(RAND_MAX)) * 0.04f;
        int treeTypeRoll = rand() % 3;
        tree.type = (treeTypeRoll == 0) ? TREE_LAYERED : (treeTypeRoll == 1) ? TREE_DEAD : TREE_TWISTED;
        trees.push_back(tree);
      }
    }
  }
  
  // West edge (left)
  for (int offset = 6; offset < blockSize - 6; offset += itemSpacing) {
    double posZ = block.worldZ + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    
    if (rand() % 2 == 0) {
      // Street lamp on sidewalk
      StreetLamp lamp;
      lamp.x = block.worldX + sidewalkWidth * 0.8;  // On sidewalk
      lamp.z = posZ;
      lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
      lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
      lamp.isWorking = (rand() % 100) < 65;
      block.lampIndices.push_back(streetLamps.size());
      streetLamps.push_back(lamp);
    } else {
      // Tree inside block - check for collision with buildings
      double treeX = block.worldX + sidewalkWidth + innerMargin;
      double treeZ = posZ;
      
      bool tooCloseToBuilding = false;
      for (const auto& building : candidates) {
        double dx = treeX - building.x;
        double dz = treeZ - building.z;
        double distance = sqrt(dx*dx + dz*dz);
        double maxDim = fmax(building.width, building.depth);
        
        if (distance < maxDim + minTreeDistance) {
          tooCloseToBuilding = true;
          break;
        }
      }
      
      if (!tooCloseToBuilding) {
        Tree tree;
        tree.x = treeX;
        tree.z = treeZ;
        tree.height = 6.0 + (rand() / double(RAND_MAX)) * 6.0;
        tree.scale = 1.0f + (rand() / float(RAND_MAX)) * 0.5f;
        tree.trunkR = 0.12f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.trunkG = 0.10f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.trunkB = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
        tree.leavesR = 0.08f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.leavesG = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
        tree.leavesB = 0.06f + (rand() / float(RAND_MAX)) * 0.04f;
        int treeTypeRoll = rand() % 3;
        tree.type = (treeTypeRoll == 0) ? TREE_LAYERED : (treeTypeRoll == 1) ? TREE_DEAD : TREE_TWISTED;
        trees.push_back(tree);
      }
    }
  }
  
  // East edge (right)
  for (int offset = 6; offset < blockSize - 6; offset += itemSpacing) {
    double posZ = block.worldZ + offset + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    
    if (rand() % 2 == 0) {
      // Street lamp on sidewalk
      StreetLamp lamp;
      lamp.x = block.worldX + blockSize - sidewalkWidth * 0.8;  // On sidewalk
      lamp.z = posZ;
      lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
      lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
      lamp.isWorking = (rand() % 100) < 65;
      block.lampIndices.push_back(streetLamps.size());
      streetLamps.push_back(lamp);
    } else {
      // Tree inside block - check for collision with buildings
      double treeX = block.worldX + blockSize - sidewalkWidth - innerMargin;
      double treeZ = posZ;
      
      bool tooCloseToBuilding = false;
      for (const auto& building : candidates) {
        double dx = treeX - building.x;
        double dz = treeZ - building.z;
        double distance = sqrt(dx*dx + dz*dz);
        double maxDim = fmax(building.width, building.depth);
        
        if (distance < maxDim + minTreeDistance) {
          tooCloseToBuilding = true;
          break;
        }
      }
      
      if (!tooCloseToBuilding) {
        Tree tree;
        tree.x = treeX;
        tree.z = treeZ;
        tree.height = 6.0 + (rand() / double(RAND_MAX)) * 6.0;
        tree.scale = 1.0f + (rand() / float(RAND_MAX)) * 0.5f;
        tree.trunkR = 0.12f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.trunkG = 0.10f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.trunkB = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
        tree.leavesR = 0.08f + (rand() / float(RAND_MAX)) * 0.05f;
        tree.leavesG = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
        tree.leavesB = 0.06f + (rand() / float(RAND_MAX)) * 0.04f;
        int treeTypeRoll = rand() % 3;
        tree.type = (treeTypeRoll == 0) ? TREE_LAYERED : (treeTypeRoll == 1) ? TREE_DEAD : TREE_TWISTED;
        trees.push_back(tree);
      }
    }
  }
}

// ============================================================================
// PARK BLOCK GENERATION
// ============================================================================

void generateParkBlock(CityBlock& block) {
  double parkCenterX = block.worldX + blockSize / 2.0;
  double parkCenterZ = block.worldZ + blockSize / 2.0;
  
  // Generate tree clusters at edges (4 corner areas)
  int numTreeClusters = 4;
  
  for (int cluster = 0; cluster < numTreeClusters; cluster++) {
    double clusterX, clusterZ;
    double edgeDistance = blockSize * 0.35;
    
    // Position clusters at corners
    switch(cluster) {
      case 0:  // North-East
        clusterX = parkCenterX + edgeDistance;
        clusterZ = parkCenterZ - edgeDistance;
        break;
      case 1:  // South-East
        clusterX = parkCenterX + edgeDistance;
        clusterZ = parkCenterZ + edgeDistance;
        break;
      case 2:  // South-West
        clusterX = parkCenterX - edgeDistance;
        clusterZ = parkCenterZ + edgeDistance;
        break;
      case 3:  // North-West
        clusterX = parkCenterX - edgeDistance;
        clusterZ = parkCenterZ - edgeDistance;
        break;
      default:
        clusterX = parkCenterX;
        clusterZ = parkCenterZ;
        break;
    }
    
    // One larger, detailed tree per cluster
    Tree tree;
    
    tree.x = clusterX + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    tree.z = clusterZ + (rand() / double(RAND_MAX) - 0.5) * 2.0;
    
    // Keep tree within park bounds
    double margin = 4.0;
    if (tree.x < block.worldX + margin) tree.x = block.worldX + margin;
    if (tree.x > block.worldX + blockSize - margin) tree.x = block.worldX + blockSize - margin;
    if (tree.z < block.worldZ + margin) tree.z = block.worldZ + margin;
    if (tree.z > block.worldZ + blockSize - margin) tree.z = block.worldZ + blockSize - margin;
    
    // Tree properties (larger as single specimens)
    tree.height = 6.0 + (rand() / double(RAND_MAX)) * 6.0;
    tree.scale = 1.2f + (rand() / float(RAND_MAX)) * 0.8f;
    
    // Dark, dead-looking colors for horror aesthetic
    tree.trunkR = 0.12f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.trunkG = 0.10f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.trunkB = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
    
    tree.leavesR = 0.08f + (rand() / float(RAND_MAX)) * 0.05f;
    tree.leavesG = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
    tree.leavesB = 0.06f + (rand() / float(RAND_MAX)) * 0.04f;
    
    // Random tree type distribution: 30% layered, 45% dead, 25% twisted
    int typeRoll = rand() % 100;
    if (typeRoll < 30) {
      tree.type = TREE_LAYERED;
    } else if (typeRoll < 75) {
      tree.type = TREE_DEAD;
    } else {
      tree.type = TREE_TWISTED;
    }
    
    trees.push_back(tree);
  }
  
  // Generate benches near edges facing center
  int numBenches = 6 + (rand() % 3);
  
  for (int i = 0; i < numBenches; i++) {
    Bench bench;
    
    // Place benches close to edges
    int placement = rand() % 4;
    double closeMargin = blockSize * 0.12;
    double alongEdge = blockSize * 0.2 + (rand() / double(RAND_MAX)) * (blockSize * 0.6);
    
    switch(placement) {
      case 0:  // North edge, facing south
        bench.x = block.worldX + alongEdge;
        bench.z = block.worldZ + closeMargin;
        bench.rotation = 0.0;
        break;
      case 1:  // South edge, facing north
        bench.x = block.worldX + alongEdge;
        bench.z = block.worldZ + blockSize - closeMargin;
        bench.rotation = 180.0;
        break;
      case 2:  // East edge, facing west
        bench.x = block.worldX + blockSize - closeMargin;
        bench.z = block.worldZ + alongEdge;
        bench.rotation = 270.0;
        break;
      case 3:  // West edge, facing east
        bench.x = block.worldX + closeMargin;
        bench.z = block.worldZ + alongEdge;
        bench.rotation = 90.0;
        break;
    }
    
    benches.push_back(bench);
  }
  
  // Add atmospheric park lighting in a ring pattern
  int numParkLights = 6;
  
  for (int i = 0; i < numParkLights; i++) {
    StreetLamp lamp;
    
    // Position lights in hexagonal ring around center
    double angle = (i / (double)numParkLights) * 2.0 * M_PI;
    double ringRadius = blockSize * 0.28;
    
    lamp.x = parkCenterX + cos(angle) * ringRadius;
    lamp.z = parkCenterZ + sin(angle) * ringRadius;
    
    // Add small random offset
    lamp.x += (rand() / double(RAND_MAX) - 0.5) * 2.0;
    lamp.z += (rand() / double(RAND_MAX) - 0.5) * 2.0;
    
    // Shorter, atmospheric park lights
    lamp.height = 3.0 + (rand() / double(RAND_MAX)) * 1.0;
    lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
    
    // Higher chance of working lights in parks (safer feeling)
    lamp.isWorking = (rand() % 100) < 75;
    
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
  }
  
  // Add sidewalk lamps around block perimeter
  addSidewalkLamps(block);
}

// ============================================================================
// INDUSTRIAL BLOCK GENERATION
// ============================================================================

void generateIndustrialBlock(CityBlock& block) {
  // 1-2 large warehouse buildings
  int numWarehouses = 1 + (rand() % 2);
  
  // Calculate safe boundaries accounting for fences
  double fenceMargin = blockSize * 0.1;  // Fence position
  
  for (int i = 0; i < numWarehouses; i++) {
    Building b;
    
    // Smaller warehouses to fit within fenced area
    // Max building dimension should be less than (blockSize - 2*safeMargin)
    b.width = 4.0 + (rand() / double(RAND_MAX)) * 3.0;   // 4-7 units (was 6-12)
    b.depth = 4.0 + (rand() / double(RAND_MAX)) * 3.0;   // 4-7 units (was 6-12)
    b.height = 10.0 + (rand() / double(RAND_MAX)) * 8.0; // Height unchanged
    
    // Grid-aligned rotation
    b.rotation = (rand() % 4) * 90.0;
    
    // Position with spacing, keeping well inside fence boundaries
    if (numWarehouses == 1) {
      // Center the single warehouse
      b.x = block.worldX + blockSize / 2.0;
      b.z = block.worldZ + blockSize / 2.0;
    } else {
      // Position two warehouses with spacing
      if (i == 0) {
        b.x = block.worldX + blockSize * 0.35;
        b.z = block.worldZ + blockSize / 2.0;
      } else {
        b.x = block.worldX + blockSize * 0.65;
        b.z = block.worldZ + blockSize / 2.0;
      }
    }
    
    // Industrial colors (dark grays, rusted browns)
    float baseVal = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
    b.r = baseVal + (rand() / float(RAND_MAX)) * 0.03f;
    b.g = baseVal - 0.02f + (rand() / float(RAND_MAX)) * 0.02f;
    b.b = baseVal - 0.03f + (rand() / float(RAND_MAX)) * 0.02f;
    
    b.buildingType = 1;
    b.hasWindows = (rand() % 100) < 60;
    b.windowPattern = 0;
    
    block.buildingIndices.push_back(buildings.size());
    buildings.push_back(b);
  }
  
  // Add 2-4 smokestacks
  int numStacks = 2 + (rand() % 3);
  for (int i = 0; i < numStacks; i++) {
    Smokestack stack;
    
    // Position around buildings
    double angle = (i / (double)numStacks) * 2.0 * M_PI;
    double distance = blockSize * 0.3;
    
    stack.x = block.worldX + blockSize / 2.0 + cos(angle) * distance;
    stack.z = block.worldZ + blockSize / 2.0 + sin(angle) * distance;
    
    stack.height = 15.0 + (rand() / double(RAND_MAX)) * 10.0;
    stack.radius = 0.8 + (rand() / double(RAND_MAX)) * 0.6;
    
    smokestacks.push_back(stack);
  }
  
  // Add chain-link fence around perimeter (reuse fenceMargin from above)
  double fenceHeight = 3.0;
  
  Fence northFence;
  northFence.x1 = block.worldX + fenceMargin;
  northFence.z1 = block.worldZ + fenceMargin;
  northFence.x2 = block.worldX + blockSize - fenceMargin;
  northFence.z2 = block.worldZ + fenceMargin;
  northFence.height = fenceHeight;
  fences.push_back(northFence);
  
  Fence southFence;
  southFence.x1 = block.worldX + fenceMargin;
  southFence.z1 = block.worldZ + blockSize - fenceMargin;
  southFence.x2 = block.worldX + blockSize - fenceMargin;
  southFence.z2 = block.worldZ + blockSize - fenceMargin;
  southFence.height = fenceHeight;
  fences.push_back(southFence);
  
  Fence eastFence;
  eastFence.x1 = block.worldX + blockSize - fenceMargin;
  eastFence.z1 = block.worldZ + fenceMargin;
  eastFence.x2 = block.worldX + blockSize - fenceMargin;
  eastFence.z2 = block.worldZ + blockSize - fenceMargin;
  eastFence.height = fenceHeight;
  fences.push_back(eastFence);
  
  Fence westFence;
  westFence.x1 = block.worldX + fenceMargin;
  westFence.z1 = block.worldZ + fenceMargin;
  westFence.x2 = block.worldX + fenceMargin;
  westFence.z2 = block.worldZ + blockSize - fenceMargin;
  westFence.height = fenceHeight;
  fences.push_back(westFence);
  
  // Minimal lighting (very dark)
  int numLights = 2;
  for (int i = 0; i < numLights; i++) {
    StreetLamp lamp;
    
    // Position at opposite corners
    if (i == 0) {
      lamp.x = block.worldX + blockSize * 0.2;
      lamp.z = block.worldZ + blockSize * 0.2;
    } else {
      lamp.x = block.worldX + blockSize * 0.8;
      lamp.z = block.worldZ + blockSize * 0.8;
    }
    
    lamp.height = 6.0 + (rand() / double(RAND_MAX)) * 2.0;
    lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
    lamp.isWorking = (rand() % 100) < 40;  // Only 40% working
    
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
  }
  
  // Add sidewalk lamps around block perimeter
  addSidewalkLamps(block);
}

// ============================================================================
// GRAVEYARD BLOCK GENERATION
// ============================================================================

void generateGraveyardBlock(CityBlock& block) {
  double graveyardCenterX = block.worldX + blockSize / 2.0;
  double graveyardCenterZ = block.worldZ + blockSize / 2.0;
  
  // Add 1-2 mausoleums as focal points
  int numMausoleums = 1 + (rand() % 2);
  
  for (int i = 0; i < numMausoleums; i++) {
    Mausoleum m;
    
    m.width = 3.0 + (rand() / double(RAND_MAX)) * 2.0;
    m.depth = 3.0 + (rand() / double(RAND_MAX)) * 2.0;
    m.height = 4.0 + (rand() / double(RAND_MAX)) * 3.0;
    
    if (numMausoleums == 1) {
      m.x = graveyardCenterX;
      m.z = graveyardCenterZ;
    } else {
      if (i == 0) {
        m.x = graveyardCenterX - blockSize * 0.2;
        m.z = graveyardCenterZ;
      } else {
        m.x = graveyardCenterX + blockSize * 0.2;
        m.z = graveyardCenterZ;
      }
    }
    
    m.rotation = (rand() % 4) * 90.0;
    
    mausoleums.push_back(m);
  }
  
  // Add gravestones in rows (classic cemetery layout)
  int numRows = 5 + (rand() % 3);
  int stonesPerRow = 4 + (rand() % 3);
  
  double rowSpacing = blockSize * 0.7 / numRows;
  double stoneSpacing = blockSize * 0.7 / stonesPerRow;
  
  double startX = block.worldX + blockSize * 0.15;
  double startZ = block.worldZ + blockSize * 0.15;
  
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < stonesPerRow; col++) {
      Gravestone stone;
      
      stone.x = startX + col * stoneSpacing + (rand() / double(RAND_MAX) - 0.5) * 0.8;
      stone.z = startZ + row * rowSpacing + (rand() / double(RAND_MAX) - 0.5) * 0.8;
      
      // Check distance to mausoleums
      bool tooClose = false;
      for (const auto& m : mausoleums) {
        double dist = sqrt((stone.x - m.x)*(stone.x - m.x) + (stone.z - m.z)*(stone.z - m.z));
        if (dist < 4.0) {
          tooClose = true;
          break;
        }
      }
      
      if (tooClose) continue;
      
      stone.width = 0.4 + (rand() / double(RAND_MAX)) * 0.3;
      stone.depth = 0.15 + (rand() / double(RAND_MAX)) * 0.1;
      stone.height = 1.0 + (rand() / double(RAND_MAX)) * 1.5;
      
      stone.rotation = (rand() / double(RAND_MAX) - 0.5) * 30.0;
      stone.stoneType = rand() % 4;
      
      gravestones.push_back(stone);
    }
  }
  
  // Add 3-5 dead trees scattered around
  int numTrees = 3 + (rand() % 3);
  
  for (int i = 0; i < numTrees; i++) {
    Tree tree;
    
    // Position at edges/corners
    double angle = (i / (double)numTrees) * 2.0 * M_PI + (rand() / double(RAND_MAX)) * 0.5;
    double distance = blockSize * (0.3 + (rand() / double(RAND_MAX)) * 0.15);
    
    tree.x = graveyardCenterX + cos(angle) * distance;
    tree.z = graveyardCenterZ + sin(angle) * distance;
    
    // Keep within bounds
    double margin = 3.0;
    if (tree.x < block.worldX + margin) tree.x = block.worldX + margin;
    if (tree.x > block.worldX + blockSize - margin) tree.x = block.worldX + blockSize - margin;
    if (tree.z < block.worldZ + margin) tree.z = block.worldZ + margin;
    if (tree.z > block.worldZ + blockSize - margin) tree.z = block.worldZ + blockSize - margin;
    
    tree.height = 7.0 + (rand() / double(RAND_MAX)) * 5.0;
    tree.scale = 1.0f + (rand() / float(RAND_MAX)) * 0.5f;
    
    // Very dark, dead colors
    tree.trunkR = 0.08f + (rand() / float(RAND_MAX)) * 0.04f;
    tree.trunkG = 0.06f + (rand() / float(RAND_MAX)) * 0.03f;
    tree.trunkB = 0.05f + (rand() / float(RAND_MAX)) * 0.02f;
    
    tree.leavesR = 0.05f;
    tree.leavesG = 0.05f;
    tree.leavesB = 0.05f;
    
    tree.type = TREE_DEAD;  // Always dead trees for graveyards
    
    trees.push_back(tree);
  }
  
  // Add wrought iron fence around perimeter
  double fenceMargin = blockSize * 0.08;
  double fenceHeight = 2.5;
  
  Fence northFence;
  northFence.x1 = block.worldX + fenceMargin;
  northFence.z1 = block.worldZ + fenceMargin;
  northFence.x2 = block.worldX + blockSize - fenceMargin;
  northFence.z2 = block.worldZ + fenceMargin;
  northFence.height = fenceHeight;
  fences.push_back(northFence);
  
  Fence southFence;
  southFence.x1 = block.worldX + fenceMargin;
  southFence.z1 = block.worldZ + blockSize - fenceMargin;
  southFence.x2 = block.worldX + blockSize - fenceMargin;
  southFence.z2 = block.worldZ + blockSize - fenceMargin;
  southFence.height = fenceHeight;
  fences.push_back(southFence);
  
  Fence eastFence;
  eastFence.x1 = block.worldX + blockSize - fenceMargin;
  eastFence.z1 = block.worldZ + fenceMargin;
  eastFence.x2 = block.worldX + blockSize - fenceMargin;
  eastFence.z2 = block.worldZ + blockSize - fenceMargin;
  eastFence.height = fenceHeight;
  fences.push_back(eastFence);
  
  Fence westFence;
  westFence.x1 = block.worldX + fenceMargin;
  westFence.z1 = block.worldZ + fenceMargin;
  westFence.x2 = block.worldX + fenceMargin;
  westFence.z2 = block.worldZ + blockSize - fenceMargin;
  westFence.height = fenceHeight;
  fences.push_back(westFence);
  
  // Very minimal lighting (super dark) - single entrance lamp
  StreetLamp lamp;
  lamp.x = block.worldX + blockSize / 2.0;
  lamp.z = block.worldZ + blockSize * 0.1;  // Near front edge
  lamp.height = 4.0 + (rand() / double(RAND_MAX)) * 1.0;
  lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
  lamp.isWorking = (rand() % 100) < 30;  // Only 30% working
  
  block.lampIndices.push_back(streetLamps.size());
  streetLamps.push_back(lamp);
  
  // Add sidewalk lamps around block perimeter
  addSidewalkLamps(block);
}

// ============================================================================
// FOREST BLOCK GENERATION
// ============================================================================

void generateForestBlock(CityBlock& block) {
  double forestCenterX = block.worldX + blockSize / 2.0;
  double forestCenterZ = block.worldZ + blockSize / 2.0;
  
  // Dense forest with 15-20 trees of all types
  int numTrees = 15 + (rand() % 6);
  
  for (int i = 0; i < numTrees; i++) {
    Tree tree;
    
    // Distribute trees throughout the block with some clustering
    // Use a mix of random and semi-grid positioning for natural look
    if (i < 8) {
      // First 8 trees: scattered throughout with good coverage
      double angle = (i / 8.0) * 2.0 * M_PI + (rand() / double(RAND_MAX) - 0.5) * 1.0;
      double distance = (rand() / double(RAND_MAX)) * blockSize * 0.35;
      tree.x = forestCenterX + cos(angle) * distance;
      tree.z = forestCenterZ + sin(angle) * distance;
    } else {
      // Remaining trees: fill in gaps randomly
      tree.x = block.worldX + 3.0 + (rand() / double(RAND_MAX)) * (blockSize - 6.0);
      tree.z = block.worldZ + 3.0 + (rand() / double(RAND_MAX)) * (blockSize - 6.0);
    }
    
    // Varied tree heights for a natural forest canopy
    tree.height = 5.0 + (rand() / double(RAND_MAX)) * 8.0;  // 5-13 units
    tree.scale = 0.8f + (rand() / float(RAND_MAX)) * 1.0f;  // 0.8-1.8 scale
    
    // Mix of healthy and dead-looking trees for horror atmosphere
    int healthRoll = rand() % 100;
    if (healthRoll < 30) {
      // Healthy darker green trees (30%)
      tree.trunkR = 0.12f + (rand() / float(RAND_MAX)) * 0.06f;
      tree.trunkG = 0.10f + (rand() / float(RAND_MAX)) * 0.06f;
      tree.trunkB = 0.08f + (rand() / float(RAND_MAX)) * 0.04f;
      
      tree.leavesR = 0.10f + (rand() / float(RAND_MAX)) * 0.08f;
      tree.leavesG = 0.18f + (rand() / float(RAND_MAX)) * 0.10f;
      tree.leavesB = 0.08f + (rand() / float(RAND_MAX)) * 0.06f;
    } else if (healthRoll < 70) {
      // Sickly/dying trees (40%)
      tree.trunkR = 0.14f + (rand() / float(RAND_MAX)) * 0.04f;
      tree.trunkG = 0.12f + (rand() / float(RAND_MAX)) * 0.04f;
      tree.trunkB = 0.10f + (rand() / float(RAND_MAX)) * 0.03f;
      
      tree.leavesR = 0.12f + (rand() / float(RAND_MAX)) * 0.04f;
      tree.leavesG = 0.14f + (rand() / float(RAND_MAX)) * 0.04f;
      tree.leavesB = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
    } else {
      // Dead/skeletal trees (30%)
      tree.trunkR = 0.10f + (rand() / float(RAND_MAX)) * 0.04f;
      tree.trunkG = 0.08f + (rand() / float(RAND_MAX)) * 0.03f;
      tree.trunkB = 0.06f + (rand() / float(RAND_MAX)) * 0.02f;
      
      tree.leavesR = 0.06f;
      tree.leavesG = 0.06f;
      tree.leavesB = 0.06f;
    }
    
    // Equal distribution of all three tree types
    int treeTypeRoll = rand() % 3;
    if (treeTypeRoll == 0) {
      tree.type = TREE_LAYERED;
    } else if (treeTypeRoll == 1) {
      tree.type = TREE_DEAD;
    } else {
      tree.type = TREE_TWISTED;
    }
    
    trees.push_back(tree);
  }
  
  // Add minimal atmospheric lighting - just a couple broken lamps
  int numLights = 1 + (rand() % 2);  // 1-2 lamps total
  
  for (int i = 0; i < numLights; i++) {
    StreetLamp lamp;
    
    // Position near edges
    if (i == 0) {
      lamp.x = block.worldX + blockSize * 0.2;
      lamp.z = block.worldZ + blockSize * 0.2;
    } else {
      lamp.x = block.worldX + blockSize * 0.8;
      lamp.z = block.worldZ + blockSize * 0.8;
    }
    
    lamp.height = 5.0 + (rand() / double(RAND_MAX)) * 1.5;
    lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
    lamp.isWorking = (rand() % 100) < 25;  // Only 25% working - very dark forest!
    
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
  }
  
  // Add sidewalk lamps around block perimeter
  addSidewalkLamps(block);
}

// ============================================================================
// CITY GRID INITIALIZATION
// ============================================================================

void initializeCityGrid() {
  cityBlocks.clear();
  buildings.clear();
  trees.clear();
  benches.clear();
  smokestacks.clear();
  fences.clear();
  gravestones.clear();
  mausoleums.clear();
  
  int halfGrid = cityGridSize / 2;
  
  for (int gx = -halfGrid; gx <= halfGrid; gx++) {
    for (int gz = -halfGrid; gz <= halfGrid; gz++) {
      CityBlock block;
      block.gridX = gx;
      block.gridZ = gz;
      
      gridToWorld(gx, gz, block.worldX, block.worldZ);
      
      // Showcase blocks near spawn - statically place one of each type for demonstration
      // Center 3x3 grid: player spawn (0,0) surrounded by example blocks
      if (gx == -1 && gz == -1) {
        // Northwest: Building block
        block.type = BLOCK_BUILDING;
        generateBuildingBlock(block);
      }
      else if (gx == 0 && gz == -1) {
        // North: Park block
        block.type = BLOCK_PARK;
        generateParkBlock(block);
      }
      else if (gx == 1 && gz == -1) {
        // Northeast: Industrial block
        block.type = BLOCK_INDUSTRIAL;
        generateIndustrialBlock(block);
      }
      else if (gx == -1 && gz == 0) {
        // West: Graveyard block
        block.type = BLOCK_GRAVEYARD;
        generateGraveyardBlock(block);
      }
      else if (gx == 0 && gz == 0) {
        // Center: Player spawn - empty
        block.type = BLOCK_EMPTY;
      }
      else if (gx == 1 && gz == 0) {
        // East: Forest block
        block.type = BLOCK_FOREST;
        generateForestBlock(block);
      }
      else if (gx == -1 && gz == 1) {
        // Southwest: Building block (second example)
        block.type = BLOCK_BUILDING;
        generateBuildingBlock(block);
      }
      else if (gx == 0 && gz == 1) {
        // South: Park block (second example)
        block.type = BLOCK_PARK;
        generateParkBlock(block);
      }
      else if (gx == 1 && gz == 1) {
        // Southeast: Empty (for breathing room)
        block.type = BLOCK_EMPTY;
      }
      else {
        // All other blocks: random distribution
        // Block distribution: 50% buildings, 15% parks, 15% industrial, 10% graveyards, 10% forest
        int roll = rand() % 100;
        if (roll < 50) {
          block.type = BLOCK_BUILDING;
          generateBuildingBlock(block);
        } else if (roll < 65) {
          block.type = BLOCK_PARK;
          generateParkBlock(block);
        } else if (roll < 80) {
          block.type = BLOCK_INDUSTRIAL;
          generateIndustrialBlock(block);
        } else if (roll < 90) {
          block.type = BLOCK_GRAVEYARD;
          generateGraveyardBlock(block);
        } else {
          block.type = BLOCK_FOREST;
          generateForestBlock(block);
        }
      }
      
      cityBlocks.push_back(block);
    }
  }
  
  std::cout << "Generated " << cityBlocks.size() << " city blocks" << std::endl;
  std::cout << "Total buildings: " << buildings.size() << std::endl;
  std::cout << "Total trees: " << trees.size() << std::endl;
  std::cout << "Total benches: " << benches.size() << std::endl;
  std::cout << "Total smokestacks: " << smokestacks.size() << std::endl;
  std::cout << "Total fence segments: " << fences.size() << std::endl;
  std::cout << "Total gravestones: " << gravestones.size() << std::endl;
  std::cout << "Total mausoleums: " << mausoleums.size() << std::endl;
}

// ============================================================================
// STREET LIGHTING GENERATION
// ============================================================================

void generateRoadLights() {
  // Street lamps and trees are now generated within each block's generation function
  // This function is kept for compatibility but does nothing
  std::cout << "Street lamps and trees generated within individual blocks" << std::endl;
  std::cout << "Total lamps: " << streetLamps.size() << std::endl;
}

// ============================================================================
// AMBIENT OBJECT INITIALIZATION
// ============================================================================

void initializeAmbientObjects() {
  ambientObjects.clear();
  
  // Place objects on streets and in blocks
  for (int i = 0; i < 300; i++) {
    double x = (rand() / double(RAND_MAX) - 0.5) * worldSize * 1.5;
    double z = (rand() / double(RAND_MAX) - 0.5) * worldSize * 1.5;
    
    // Check distance to buildings
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
