#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPEED 14 /* 50km/t in m/s */
#define CAR_LENGTH 4 /* meters */
#define MAX_CARS 100
#define MAX_ROUTES 20
#define MAX_TIME_VALUES 5000

typedef struct Cars {
    double current_speed;
    double start_time;
    int route;
    float current_position;
    int driving_direction; /* Uses enum direction values */
} Cars;

struct Intersection {
    int is_traffic_light;
    int road_connections; /* 0: northwards, 1: southwards, 2: both */
};

typedef struct Car_Route {
    int start_position;
    int intersections[5];
} Car_Route;


enum direction { North, South, East, West };
Cars Create_Car(Cars *car, Car_Route *cr);
int Run_Car(Cars *car, double time, Car_Route *cr, int *All_Times);
int driving_direction(Cars car, Car_Route cr, int n);
void get_route(Car_Route *cr);
int car_turning(Cars *car, double time, Car_Route *cr, int *All_Times);
void Return_Time(int Car_Time, int *All_Times);

int main() {
    double time;
    Car_Route cr[MAX_ROUTES];
    Cars car[MAX_CARS];
    int All_Times[MAX_TIME_VALUES];
    int i;


    get_route(cr);
    for(i=0; i<MAX_CARS; i++){
    car[i] = Create_Car(car, cr);
    }

    while (Run_Car(&car[1], time, cr, All_Times) != 1) {
        time++;
    }
    printf("Test of return_time: must be equal to n's car's time: %d\n ",All_Times[0]);
    return EXIT_SUCCESS;
}

Cars Create_Car(Cars *car, Car_Route *cr) {
    car->current_position = cr->start_position;
    car->start_time = 0;
    car->route = 1;
    car->driving_direction = 2;
    return *car;
}

int Run_Car(Cars *car, double time, Car_Route *cr, int *All_Times) {
    printf("Driving direction %d ", car->driving_direction);

    car->current_speed = MAX_SPEED;
    car->current_position = car->current_position + car->current_speed;
    printf("position: %lf tid: %lf\n", car->current_position,time);

    car_turning(car, time, cr,All_Times);
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

int driving_direction(Cars car, Car_Route cr, int n) {
    switch(cr.intersections[n]) {
        case 0: return 0; break;
        case 1: return 1; break;
        case 2: return 2; break;
        case 3: return 3; break;
    }
}

int car_turning(Cars *car, double time, Car_Route *cr, int *All_Times){
    if(car->current_position > 100 && car->current_position < 125) {
        car->driving_direction = driving_direction(*car, *cr, 0);
    }
    else if(car->current_position > 200 && car->current_position < 225) {
        car->driving_direction = driving_direction(*car, *cr, 1);
        printf("Car turns time: %lf", time - car->start_time);
        Return_Time(time - car->start_time, All_Times);
        return 1;
    }
    else if (car->current_position > 800) {
        printf("Car finish time: %lf\n", time - car->start_time);
        Return_Time(time - car->start_time, All_Times);
    }
    else
        return 0;
}

void Return_Time(int Car_Time, int *All_Times)
{
    /*Static since the counter must count up 1 every time the function is called */
    static int Car_Count = 0;
    All_Times[Car_Count] = Car_Time;
    Car_Count++;

    
}