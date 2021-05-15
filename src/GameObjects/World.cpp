#include "World.h"

MapElement World::NumberToMapElement(int number) {
    switch (number)
    {
    case 0: return MapElement::None;
    case 1: return MapElement::Wall;
    case 2: return MapElement::Point;
    case 3: return MapElement::Power;
    case 4: return MapElement::Tunnel;
    }
}

void World::CalculatePositions() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            positionMap[y][x] = glm::vec3((x - WIDTH / 2 + xoff) * mazeScale, 0.5, (y - HEIGHT / 2 + yoff) * mazeScale);
        }
    }
}

glm::vec3 World::GetPosition(int x, int y){
    return positionMap[y][x];
}

MapElement World::GetMapElement(int x, int y){
    return NumberToMapElement(map[y][x]);
}

void World::SetMapElement(int x, int y, MapElement element){
    map[y][x] = (int) element;
}

bool World::IsPositionValid(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        return true;
    }

    return false;
}