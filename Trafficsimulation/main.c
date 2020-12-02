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

void get_route(Car_Route *cr);

int main() {
    Car_Route cr;
    get_route(&cr);

    return EXIT_SUCCESS;
}

void get_route(Car_Route *cr) {
    char *direction;
    int i;

    cr->start_position = 1;

    cr->intersections[0] = East; cr->intersections[1] = East; cr->intersections[2] = East; cr->intersections[3] = East; cr->intersections[4] = East;

    printf("Starting position = %d\n", cr->start_position);
    for(i = 0; i < 5; i++) {
        switch (cr->intersections[i]) {
            case 0: direction = "North"; break;
            case 1: direction = "South"; break;
            case 2: direction = "East"; break;
            case 3: direction = "West"; break;
        }
        printf("In intersection %d Go %s\n",i+1 , direction);
    }
}
