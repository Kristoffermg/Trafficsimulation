#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPEED 14 /* 50km/t in m/s */
#define CAR_LENGTH 4 /* meters */

struct Car {
    double current_speed;
    double start_time;
    int route;
    float current_position;
    int driving_direction; /* uses enum directions values */
};

struct Intersection {
    int is_traffic_light;
    int road_connections; /* 0: northwards, 1: southwards, 2: both */
};

struct Car_Route {
    int start_position;
    int intersections[5];
};

typedef struct Car_Route Car_Route;
typedef struct Car Car;


enum direction { North, South, East, West };
Car Create_Car(Car car);
int Run_Car(Car *car, double time);

void get_route(Car_Route *cr);

int main() {
    double time;
    Car_Route cr;

    Car car;
    Car car1;
    car1 = Create_Car(car);
    while (Run_Car(&car1,time) != 1)
    {
        time++;
    }
    
    return EXIT_SUCCESS;
}

Car Create_Car(Car car)
{
car.current_position = 0;
car.start_time = 0;
car.route = 5;
return car;
}

int Run_Car(Car *car, double time)
{
    if (car->current_position < 800)
    {
    car->current_speed = MAX_SPEED;
    car->current_position = car->current_position + car->current_speed;
    printf("position: %lf time: %lf\n",car->current_position,time);
    return 0;
    }
    else if (car->current_position > 800)
    {
    printf("car finish time: %lf",time - car->start_time);
    return 1;
    }
}
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
