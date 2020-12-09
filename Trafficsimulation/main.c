#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STRING_LENGTH 1000
#define MAX_SPEED 14 /* 50km/t in m/s */
#define CAR_LENGTH 4 /* meters */
#define MAX_CARS 100
#define MAX_ROUTES 1
#define MAX_INTERSECTIONS 5
#define MAX_TIME_VALUES 5000

typedef struct Cars {
    double current_speed;
    int start_time;
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
int Run_Car(Cars *car, Car_Route *cr, int *all_times);
/*int driving_direction(Cars car, Car_Route cr, int n);*/
void Get_Route(Car_Route *cr);
void Print_Route_Summary(Car_Route *cr);
int Car_Turning(Cars *car, Car_Route *cr, int *all_times);
void delay(int number_of_milli_seconds);
void Return_time(int start_time, int *all_times, int *car_count);
double Average_time(int *all_times, int *car_count);
int Car_Driving_Time(int start_time);

int global_time = 0;

int main() {
    Car_Route cr[MAX_ROUTES]; /* cr = car route */
    Cars car[MAX_CARS];
    int all_times[MAX_TIME_VALUES];
    int i;
    int car_count = 0;

    clock_t start = clock(); /* Starts global_time measurement */

    Get_Route(cr);
    for(i = 0; i < MAX_CARS; i++){
        car[i] = Create_Car(car, cr);
    }

    printf("Car init: ---------\n");
    while (Run_Car(&car[1], cr, all_times) != 1) {
        global_time++;
    }

    Return_time(car[1].start_time, all_times, &car_count);

    return EXIT_SUCCESS;
}

Cars Create_Car(Cars *car, Car_Route *cr) {
    car -> current_position = cr -> start_position;
    car -> start_time = global_time;
    car -> route = 1;
    car -> driving_direction = 2;
    return *car;
}

double prev_time = 0;

int Run_Car(Cars *car, Car_Route *cr, int *all_times) {
    printf("Driving direction %d ", car -> driving_direction);

    car -> current_speed = MAX_SPEED;
    car -> current_position += car -> current_speed * (global_time - prev_time); /* distance = speed * global_time */
    printf("Position: %lf time: %d\n", car -> current_position, global_time);
    prev_time = global_time;
    return Car_Turning(car, cr, all_times);
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
    for(i = 1; i < 5; i++) {
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
        printf("In intersection %d go %s\n", i, direction);
    }
}

int Car_Turning(Cars *car, Car_Route *cr, int *all_times) {
    if(car -> current_position >= 100 && car -> current_position <= 125) {
        car -> driving_direction = cr[0].intersections[0];
    }
    else if(car -> current_position >= 200 && car -> current_position <= 225) {
        car -> driving_direction = cr[0].intersections[1];
            if(car -> driving_direction != 2) {
                printf("Car turns at time: %d", global_time - car -> start_time);
                return 1;
            }
    }
    else if (car -> current_position >= 800) {
        printf("Car finished at time: %d", global_time - car -> start_time);
        return 1;
    }
    return 0;
}

void delay(int number_of_milli_seconds) { 
    /* Storing start global_time */
    clock_t start = clock(); 
  
    /* Looping till required global_time is not achieved */
    while (clock() < start + number_of_milli_seconds); 
} 

void Return_time(int start_time, int *all_times, int *car_count) {
    int i = *car_count;
    all_times[i] = Car_Driving_Time(start_time);
    *car_count += 1;
}

double Average_time(int *all_times, int *car_count){
    int combined_times = 0;
    double average_time;
    int car_count_toint = *car_count;
    int i;
    for (i = 0; i < car_count_toint; i++)
    {
        combined_times = combined_times + all_times[i];
    }
    average_time = combined_times / car_count_toint;
    return average_time;
}

int Car_Driving_Time(int start_time) {
    return global_time - start_time;
}