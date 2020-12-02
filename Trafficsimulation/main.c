#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPEED 14 /* 50km/t i m/s */
#define CAR_LENGTH 4 /* meter */

struct Car {
    double current_speed;
    double start_time;
    int route;
    float current_position;
    int driving_direction; /* bruger enum directions værdier */
};

struct Intersection {
    int is_traffic_light;
    int road_connections; /* 0: nordpå, 1: sydpå, 2: begge */
};

struct Car_Route {
    int start_position;
    int intersections[5];
};

typedef struct Car_Route Car_Route;

enum direction { North, South, East, West };

int main() {
    Car_Route cr;
    
    return EXIT_SUCCESS;
}

/* Test*/