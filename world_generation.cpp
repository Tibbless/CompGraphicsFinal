#include "eerie_city.h"

// Helper function to convert grid coordinates to world coordinates
void gridToWorld(int gridX, int gridZ, double& worldX, double& worldZ) {
  int totalBlockSize = blockSize + roadWidth;
  worldX = gridX * totalBlockSize;
  worldZ = gridZ * totalBlockSize;
}

// Generate a building block with multiple buildings (no overlaps)
void generateBuildingBlock(CityBlock& block) {
  // Create 3-6 buildings per block (increased from 2-5)
  int numBuildings = 3 + (rand() % 4);
  
  std::vector<Building> candidates;
  int maxAttempts = 50; // Try up to 50 times to place all buildings
  
  for (int i = 0; i < numBuildings; i++) {
    bool placed = false;
    int attempts = 0;
    
    while (!placed && attempts < maxAttempts) {
      attempts++;
      
      Building b;
      
      // Building dimensions - SMALLER sizes for more buildings per block
      b.width = 2.0 + (rand() / double(RAND_MAX)) * 3.0;   // Was 3.0-8.0, now 2.0-5.0
      b.depth = 2.0 + (rand() / double(RAND_MAX)) * 3.0;   // Was 3.0-8.0, now 2.0-5.0
      b.height = 8.0 + (rand() / double(RAND_MAX)) * 28.0; // Height unchanged
      
      // Rotation - align mostly to grid with some variation
      b.rotation = (rand() % 4) * 90.0 + (rand() / double(RAND_MAX) - 0.5) * 10.0;
      
      // Calculate the maximum dimension for this building (accounting for rotation)
      double maxDim = fmax(b.width, b.depth);
      
      // Position within the block (leaving margins for streets AND building size)
      double marginX = blockSize * 0.15 + maxDim; // Street margin + building radius
      double marginZ = blockSize * 0.15 + maxDim;
      double usableWidth = blockSize - (2 * marginX);
      double usableDepth = blockSize - (2 * marginZ);
      
      // Make sure we have room to place buildings
      if (usableWidth <= 0 || usableDepth <= 0) {
        // Building is too big for this block, skip it
        break;
      }
      
      b.x = block.worldX + marginX + (rand() / double(RAND_MAX)) * usableWidth;
      b.z = block.worldZ + marginZ + (rand() / double(RAND_MAX)) * usableDepth;
      
      // Desaturated, dark colors for PS1 horror aesthetic
      float baseVal = 0.15f + (rand() / float(RAND_MAX)) * 0.15f;
      b.r = baseVal + (rand() / float(RAND_MAX)) * 0.05f;
      b.g = baseVal + (rand() / float(RAND_MAX)) * 0.05f;
      b.b = baseVal + (rand() / float(RAND_MAX)) * 0.08f;
      
      b.buildingType = rand() % 3;
      b.hasWindows = (rand() % 100) < 95;  // INCREASED from 75 to 95 - almost all buildings have windows now
      b.windowPattern = rand() % 4;
      
      // Check if building stays within block boundaries
      bool outsideBlock = false;
      if (b.x - maxDim < block.worldX ||
          b.x + maxDim > block.worldX + blockSize ||
          b.z - maxDim < block.worldZ ||
          b.z + maxDim > block.worldZ + blockSize) {
        outsideBlock = true;
      }
      
      if (outsideBlock) {
        continue; // Try again
      }
      
      // Check for overlap with existing buildings in this block
      bool overlaps = false;
      double minSeparation = 1.0; // Reduced from 1.5 to allow tighter packing
      
      for (const auto& existing : candidates) {
        // Calculate distance between building centers
        double dx = b.x - existing.x;
        double dz = b.z - existing.z;
        double distance = sqrt(dx*dx + dz*dz);
        
        // Calculate minimum safe distance (sum of half-widths + separation)
        // Use max dimension for each building to be safe with rotation
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

// Generate an industrial block with warehouses, smokestacks, and fences
void generateIndustrialBlock(CityBlock& block) {
  // Industrial blocks have 1-2 large warehouse buildings
  int numWarehouses = 1 + (rand() % 2);
  
  for (int i = 0; i < numWarehouses; i++) {
    Building b;
    
    // Warehouses are LARGE and boxy
    b.width = 6.0 + (rand() / double(RAND_MAX)) * 6.0;   // 6-12 units wide
    b.depth = 6.0 + (rand() / double(RAND_MAX)) * 6.0;   // 6-12 units deep
    b.height = 10.0 + (rand() / double(RAND_MAX)) * 8.0; // 10-18 units tall (shorter than office buildings)
    
    // Minimal rotation - industrial buildings are very grid-aligned
    b.rotation = (rand() % 4) * 90.0;
    
    // Position with good margins
    double marginX = blockSize * 0.2;
    double marginZ = blockSize * 0.2;
    double usableWidth = blockSize - (2 * marginX);
    double usableDepth = blockSize - (2 * marginZ);
    
    if (numWarehouses == 1) {
      // Single warehouse - center it
      b.x = block.worldX + blockSize / 2.0;
      b.z = block.worldZ + blockSize / 2.0;
    } else {
      // Two warehouses - position them with spacing
      if (i == 0) {
        b.x = block.worldX + blockSize * 0.35;
        b.z = block.worldZ + blockSize / 2.0;
      } else {
        b.x = block.worldX + blockSize * 0.65;
        b.z = block.worldZ + blockSize / 2.0;
      }
    }
    
    // Industrial colors - dark grays, rusted browns
    float baseVal = 0.12f + (rand() / float(RAND_MAX)) * 0.08f;
    b.r = baseVal + (rand() / float(RAND_MAX)) * 0.03f;
    b.g = baseVal - 0.02f + (rand() / float(RAND_MAX)) * 0.02f;  // Slightly less green
    b.b = baseVal - 0.03f + (rand() / float(RAND_MAX)) * 0.02f;  // Even less blue (rust)
    
    b.buildingType = 1;  // Boxy style
    b.hasWindows = (rand() % 100) < 60;  // Fewer windows than normal buildings
    b.windowPattern = 0;  // Simple pattern
    
    block.buildingIndices.push_back(buildings.size());
    buildings.push_back(b);
  }
  
  // Add 2-4 smokestacks
  int numStacks = 2 + (rand() % 3);
  for (int i = 0; i < numStacks; i++) {
    Smokestack stack;
    
    // Position near buildings but not overlapping
    double angle = (i / (double)numStacks) * 2.0 * M_PI;
    double distance = blockSize * 0.3;
    
    stack.x = block.worldX + blockSize / 2.0 + cos(angle) * distance;
    stack.z = block.worldZ + blockSize / 2.0 + sin(angle) * distance;
    
    stack.height = 15.0 + (rand() / double(RAND_MAX)) * 10.0;  // 15-25 units tall
    stack.radius = 0.8 + (rand() / double(RAND_MAX)) * 0.6;     // 0.8-1.4 units radius
    
    smokestacks.push_back(stack);
  }
  
  // Add chain-link fence around perimeter
  double fenceMargin = blockSize * 0.1;
  double fenceHeight = 3.0;
  
  // North fence
  Fence northFence;
  northFence.x1 = block.worldX + fenceMargin;
  northFence.z1 = block.worldZ + fenceMargin;
  northFence.x2 = block.worldX + blockSize - fenceMargin;
  northFence.z2 = block.worldZ + fenceMargin;
  northFence.height = fenceHeight;
  fences.push_back(northFence);
  
  // South fence
  Fence southFence;
  southFence.x1 = block.worldX + fenceMargin;
  southFence.z1 = block.worldZ + blockSize - fenceMargin;
  southFence.x2 = block.worldX + blockSize - fenceMargin;
  southFence.z2 = block.worldZ + blockSize - fenceMargin;
  southFence.height = fenceHeight;
  fences.push_back(southFence);
  
  // East fence
  Fence eastFence;
  eastFence.x1 = block.worldX + blockSize - fenceMargin;
  eastFence.z1 = block.worldZ + fenceMargin;
  eastFence.x2 = block.worldX + blockSize - fenceMargin;
  eastFence.z2 = block.worldZ + blockSize - fenceMargin;
  eastFence.height = fenceHeight;
  fences.push_back(eastFence);
  
  // West fence
  Fence westFence;
  westFence.x1 = block.worldX + fenceMargin;
  westFence.z1 = block.worldZ + fenceMargin;
  westFence.x2 = block.worldX + fenceMargin;
  westFence.z2 = block.worldZ + blockSize - fenceMargin;
  westFence.height = fenceHeight;
  fences.push_back(westFence);
  
  // Minimal lighting in industrial areas - very few working lamps
  int numLights = 2;  // Only 2 lights per industrial block
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
    lamp.isWorking = (rand() % 100) < 40;  // Only 40% working - very dark!
    
    block.lampIndices.push_back(streetLamps.size());
    streetLamps.push_back(lamp);
  }
}

// Generate a graveyard block with gravestones, mausoleums, and dead trees
void generateGraveyardBlock(CityBlock& block) {
  double graveyardCenterX = block.worldX + blockSize / 2.0;
  double graveyardCenterZ = block.worldZ + blockSize / 2.0;
  
  // Add 1-2 mausoleums as focal points
  int numMausoleums = 1 + (rand() % 2);
  
  for (int i = 0; i < numMausoleums; i++) {
    Mausoleum m;
    
    // Larger structures positioned prominently
    m.width = 3.0 + (rand() / double(RAND_MAX)) * 2.0;   // 3-5 units
    m.depth = 3.0 + (rand() / double(RAND_MAX)) * 2.0;   // 3-5 units
    m.height = 4.0 + (rand() / double(RAND_MAX)) * 3.0;  // 4-7 units
    
    if (numMausoleums == 1) {
      // Single mausoleum - center it
      m.x = graveyardCenterX;
      m.z = graveyardCenterZ;
    } else {
      // Two mausoleums - spread them out
      if (i == 0) {
        m.x = graveyardCenterX - blockSize * 0.2;
        m.z = graveyardCenterZ;
      } else {
        m.x = graveyardCenterX + blockSize * 0.2;
        m.z = graveyardCenterZ;
      }
    }
    
    m.rotation = (rand() % 4) * 90.0;  // Align to grid
    
    mausoleums.push_back(m);
  }
  
  // Add many gravestones in rows (classic cemetery layout)
  int numRows = 5 + (rand() % 3);     // 5-7 rows
  int stonesPerRow = 4 + (rand() % 3); // 4-6 stones per row
  
  double rowSpacing = blockSize * 0.7 / numRows;
  double stoneSpacing = blockSize * 0.7 / stonesPerRow;
  
  double startX = block.worldX + blockSize * 0.15;
  double startZ = block.worldZ + blockSize * 0.15;
  
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < stonesPerRow; col++) {
      Gravestone stone;
      
      stone.x = startX + col * stoneSpacing + (rand() / double(RAND_MAX) - 0.5) * 0.8;
      stone.z = startZ + row * rowSpacing + (rand() / double(RAND_MAX) - 0.5) * 0.8;
      
      // Check if too close to mausoleums
      bool tooClose = false;
      for (const auto& m : mausoleums) {
        double dist = sqrt((stone.x - m.x)*(stone.x - m.x) + (stone.z - m.z)*(stone.z - m.z));
        if (dist < 4.0) {
          tooClose = true;
          break;
        }
      }
      
      if (tooClose) continue;
      
      // Gravestone dimensions
      stone.width = 0.4 + (rand() / double(RAND_MAX)) * 0.3;   // 0.4-0.7
      stone.depth = 0.15 + (rand() / double(RAND_MAX)) * 0.1;  // 0.15-0.25
      stone.height = 1.0 + (rand() / double(RAND_MAX)) * 1.5;  // 1.0-2.5
      
      stone.rotation = (rand() / double(RAND_MAX) - 0.5) * 30.0;  // Slight random tilt
      stone.stoneType = rand() % 4;  // Random gravestone type
      
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
    
    tree.height = 7.0 + (rand() / double(RAND_MAX)) * 5.0;  // 7-12 units
    tree.scale = 1.0f + (rand() / float(RAND_MAX)) * 0.5f;
    
    // Very dark, dead colors
    tree.trunkR = 0.08f + (rand() / float(RAND_MAX)) * 0.04f;
    tree.trunkG = 0.06f + (rand() / float(RAND_MAX)) * 0.03f;
    tree.trunkB = 0.05f + (rand() / float(RAND_MAX)) * 0.02f;
    
    tree.leavesR = 0.05f;
    tree.leavesG = 0.05f;
    tree.leavesB = 0.05f;
    
    tree.type = TREE_DEAD;  // Always use dead tree type for graveyards
    
    trees.push_back(tree);
  }
  
  // Add wrought iron fence around perimeter
  double fenceMargin = blockSize * 0.08;
  double fenceHeight = 2.5;
  
  // North fence
  Fence northFence;
  northFence.x1 = block.worldX + fenceMargin;
  northFence.z1 = block.worldZ + fenceMargin;
  northFence.x2 = block.worldX + blockSize - fenceMargin;
  northFence.z2 = block.worldZ + fenceMargin;
  northFence.height = fenceHeight;
  fences.push_back(northFence);
  
  // South fence
  Fence southFence;
  southFence.x1 = block.worldX + fenceMargin;
  southFence.z1 = block.worldZ + blockSize - fenceMargin;
  southFence.x2 = block.worldX + blockSize - fenceMargin;
  southFence.z2 = block.worldZ + blockSize - fenceMargin;
  southFence.height = fenceHeight;
  fences.push_back(southFence);
  
  // East fence
  Fence eastFence;
  eastFence.x1 = block.worldX + blockSize - fenceMargin;
  eastFence.z1 = block.worldZ + fenceMargin;
  eastFence.x2 = block.worldX + blockSize - fenceMargin;
  eastFence.z2 = block.worldZ + blockSize - fenceMargin;
  eastFence.height = fenceHeight;
  fences.push_back(eastFence);
  
  // West fence
  Fence westFence;
  westFence.x1 = block.worldX + fenceMargin;
  westFence.z1 = block.worldZ + fenceMargin;
  westFence.x2 = block.worldX + fenceMargin;
  westFence.z2 = block.worldZ + blockSize - fenceMargin;
  westFence.height = fenceHeight;
  fences.push_back(westFence);
  
  // Very minimal lighting - graveyards are DARK
  int numLights = 1;  // Only 1 light at entrance
  
  StreetLamp lamp;
  lamp.x = block.worldX + blockSize / 2.0;
  lamp.z = block.worldZ + blockSize * 0.1;  // Near front edge
  lamp.height = 4.0 + (rand() / double(RAND_MAX)) * 1.0;
  lamp.flickerPhase = (rand() / double(RAND_MAX)) * 6.28;
  lamp.isWorking = (rand() % 100) < 30;  // Only 30% working - super dark!
  
  block.lampIndices.push_back(streetLamps.size());
  streetLamps.push_back(lamp);
}


// Initialize the city grid with blocks
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
      
      // Skip center blocks for player spawn area
      if (abs(gx) <= 1 && abs(gz) <= 1) {
        block.type = BLOCK_EMPTY;
      } 
      // Add variety: 55% buildings, 20% parks, 15% industrial, 10% graveyards
      else {
        int roll = rand() % 100;
        if (roll < 55) {
          block.type = BLOCK_BUILDING;
          generateBuildingBlock(block);
        } else if (roll < 75) {
          block.type = BLOCK_PARK;
          generateParkBlock(block);
        } else if (roll < 90) {
          block.type = BLOCK_INDUSTRIAL;
          generateIndustrialBlock(block);
        } else {
          block.type = BLOCK_GRAVEYARD;
          generateGraveyardBlock(block);
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
