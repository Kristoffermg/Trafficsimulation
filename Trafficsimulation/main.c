#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STRING_LENGTH 1000
#define MAX_SPEED 14 /* 50km/t in m/s */
#define CAR_LENGTH 4 /* meter */
#define MAX_CARS 100
#define MAX_ROUTES 1
#define MAX_INTERSECTIONS 5
#define MAX_TIME_VALUES 5000

typedef struct Cars {
    double current_speed;
    double start_time;
    int route;
    float current_position;
    int driving_direction; /* uses enum directions values */
} Cars;

struct Intersection {
    int is_traffic_light;
    int road_connections; /* 0: northwards, 1: southwards, 2: both */
};

typedef struct Car_Route {
    int route_number;
    int start_position;
    int intersections[MAX_INTERSECTIONS];
} Car_Route;


enum direction { North, South, East, West, OutOfSystem };

Cars Create_Car(Cars *car, Car_Route *cr);

int Run_Car(Cars *car, double time, Car_Route *cr, int *all_times);
//int driving_direction(Cars car, Car_Route cr, int n);
void Get_Route(Car_Route *cr);
void Print_Route_Summary(Car_Route *cr);
int Car_Turning(Cars *car, double time, Car_Route *cr, int *all_times);
void Return_Time(int car_time, int *all_times);
int Int_Convert(char *temp_intersections);

int main() {
    double time;
    Car_Route cr[MAX_ROUTES]; /* cr = car route */
    Cars car[MAX_CARS];
    int all_times[MAX_TIME_VALUES];
    int i;

    Get_Route(cr);
    for(i = 0; i < MAX_CARS; i++){
        car[i] = Create_Car(car, cr);
    }

    printf("Car init: ---------\n");
    while (Run_Car(&car[1], time, cr, all_times) != 1) {
        time++;
    }
    
    return EXIT_SUCCESS;
}

Cars Create_Car(Cars *car, Car_Route *cr) {
    car -> current_position = cr  ->  start_position;
    car -> start_time = 0;
    car -> route = 1;
    car -> driving_direction = 2;
    return *car;
}

int Run_Car(Cars *car, double time, Car_Route *cr, int *all_times) {
    printf("Driving direction %d ", car -> driving_direction);

    car -> current_speed = MAX_SPEED;
    car -> current_position = car -> current_position + car -> current_speed;
    printf("Position: %lf tid: %lf\n", car -> current_position,time);

    return Car_Turning(car, time, cr,all_times);
}

void Get_Route(Car_Route *cr) {
    int i;

    FILE *routes_file_pointer;
    routes_file_pointer = fopen("Routes.txt", "r");

    for(i = 0; i < MAX_ROUTES; i++) {  
        fscanf(routes_file_pointer, " %d %d %d %d %d %d %d", &cr[i].route_number, &cr[i].start_position, &cr[i].intersections[0],
                                               &cr[i].intersections[1], &cr[i].intersections[2], &cr[i].intersections[3],
                                               &cr[i].intersections[4]);
    }
    fclose(routes_file_pointer);

    Print_Route_Summary(cr);
}

void Print_Route_Summary(Car_Route *cr) {
    char* direction;
    int i;

    printf("Route number = %d\n", cr[0].route_number);
    printf("Starting position = %d\n", cr[0].start_position);
    for(i = 0; i < 5; i++) {
        if(cr[0].intersections[i] == OutOfSystem) {
            printf("Done with route, out of system.\n");
            printf("Route end: --------- \n\n");
            break;
        }

        switch (cr[0].intersections[i]) {
            case 0: direction = "North"; break;
            case 1: direction = "South"; break;
            case 2: direction = "East"; break;
            case 3: direction = "West"; break;
        }
        printf("In intersection %d go %s\n", i+1, direction);
    }
}

int Car_Turning(Cars *car, double time, Car_Route *cr, int *all_times) {
    if(car -> current_position >= 100 && car -> current_position <= 125) {
        car -> driving_direction = cr[0].intersections[0];
    }
    else if(car -> current_position >= 200 && car -> current_position <= 225) {
        car -> driving_direction = cr[0].intersections[1];
            if(car -> driving_direction != 2) {
                printf("Car turns at time: %lf", time - car -> start_time);
                return 1;
            }
    }
    else if (car -> current_position >= 800) {
        printf("Car finished at time: %lf", time - car -> start_time);
        return 1;
    }
    return 0;
}

void Return_Time(int car_time, int *all_times) {
    /* Static since the counter should count up 1 every time the function is called */
    static int car_count = 0;
    all_times[car_count] = car_time;
    car_count++;
}

int Int_Convert(char *temp_intersections) {
    return atoi(temp_intersections);   
}