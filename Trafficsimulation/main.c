#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPEED 14 /* 50km/t i m/s */
#define CAR_LENGTH 4 /* meter */

typedef struct Car {
    double current_speed;
    double start_time;
    int route;
    float current_position;
    int driving_direction; /* bruger enum directions værdier */
} Car;

struct Intersection {
    int is_traffic_light;
    int road_connections; /* 0: nordpå, 1: sydpå, 2: begge */
};

typedef struct Car_Route {
    int start_position;
    int intersections[5];
} Car_Route;


enum direction { North, South, East, West };
Car Create_Car(Car car, Car_Route *cr);
int Run_Car(Car *car, double time, Car_Route *cr);
int driving_direction(Car car, Car_Route cr, int n);

void get_route(Car_Route *cr);

int main() {
    double time;
    Car_Route cr;
    Car car;
    Car car1;

    get_route(&cr);

    car = Create_Car(car, &cr);

    while (Run_Car(&car, time, &cr) != 1) {
        time++;
    }
    
    return EXIT_SUCCESS;
}

Car Create_Car(Car car, Car_Route *cr) {
    car.current_position = cr->start_position;
    car.start_time = 0;
    car.route = 1;
    car.driving_direction = 2;
    return car;
}

int Run_Car(Car *car, double time, Car_Route *cr) {
    printf("Driving direction %d ", car->driving_direction);

    car->current_speed = MAX_SPEED;
    car->current_position = car->current_position + car->current_speed;
    printf("position: %lf tid: %lf\n", car->current_position,time);

    if(car->current_position > 100 && car->current_position < 125) {
        car->driving_direction = driving_direction(*car, *cr, 0);
    }
    else if(car->current_position > 200 && car->current_position < 225) {
        car->driving_direction = driving_direction(*car, *cr, 1);
        printf("bil drejer tid: %lf", time - car->start_time);
        return 1;
    }
    else if (car->current_position > 800) {
        printf("bil færdig tid: %lf", time - car->start_time);
    }
    else
        return 0;
}

void get_route(Car_Route *cr) {
    char *direction;
    int i;

    cr->start_position = 0;

    cr->intersections[0] = East; cr->intersections[1] = North; cr->intersections[2] = East; cr->intersections[3] = East; cr->intersections[4] = East;

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

int driving_direction(Car car, Car_Route cr, int n) {
    switch(cr.intersections[n]) {
        case 0: return 0; break;
        case 1: return 1; break;
        case 2: return 2; break;
        case 3: return 3; break;
    }
}
