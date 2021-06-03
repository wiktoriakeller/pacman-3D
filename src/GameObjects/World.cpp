#include "World.h"

MapElement World::NumberToMapElement(int number) {
    switch (number)
    {
    case 0: return MapElement::None;
    case 1: return MapElement::Wall;
    case 2: return MapElement::Point;
    case 3: return MapElement::Power;
    case 4: return MapElement::Tunnel;
    case 5: return MapElement::MissingPoint;
    case 6: return MapElement::MissingPower;
    case 7: return MapElement::Ghost;
    case 8: return MapElement::Cherry;
    }
}

void World::CalculatePositions() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            positionMap[y][x] = glm::vec3((x - WIDTH / 2 + XOFF) * MAZE_SCALE, 0.5, (y - HEIGHT / 2 + YOFF) * MAZE_SCALE);
        }
    }
    WorldCenter = (positionMap[17][WIDTH / 2 - 1] + positionMap[17][WIDTH / 2]) / 2.0f;
}

bool World::ArePositionsEqual(glm::vec3 position1, glm::vec3 position2) const {
    if (abs(position1.x - position2.x) > EPSILON || abs(position1.z - position2.z) > EPSILON) {
        return false;
    }
    return true;
}

glm::vec3 World::GetPosition(int x, int y) const {
    return positionMap[y][x];
}

MapElement World::GetMapElement(int x, int y) {
    return NumberToMapElement(map[y][x]);
}

void World::SetMapElement(int x, int y, MapElement element) {
    map[y][x] = (int)element;
}

bool World::IsPositionValid(int x, int y) const {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        return true;
    }
    return false;
}