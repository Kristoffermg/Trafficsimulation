#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_STRING_LENGTH 1000
#define MAX_SPEED 14 /* 50km/t i m/s */
#define CAR_LENGTH 4 /* meter */
#define MAX_CARS 100
#define MAX_ROUTES 3
#define MAX_INTERSECTIONS 5
#define MAX_TIME_VALUES 5000

typedef struct Cars {
    float current_speed;
    int start_time;
    int route;
    float current_position;
    int driving_direction; /* bruger enum directions værdier */
} Cars;

struct Intersection {
    int is_traffic_light;
    int road_connections; /* 0: nordpå, 1: sydpå, 2: begge */
};

typedef struct Car_Route {
    int route_number;
    int start_position;
    int intersections[MAX_INTERSECTIONS];
} Car_Route;

typedef struct Thread_Args{
    Cars car;
    Car_Route cr;
    int *time;
    int i;
} Thread_Args;

enum direction { North, South, East, West, OutOfSystem };

Thread_Args Update_Car_Thread(Cars car, Car_Route cr, int *time, int i);
Cars Create_Car(Cars *car, Car_Route *cr);
void *Run_Car(void *_args);
void Get_Route(Car_Route *cr);
void Print_Route_Summary(Car_Route cr, int i);
int Car_Turning(Cars car, int time, Car_Route cr, int *all_times, int i);
void Return_Time(int car_time, int *all_times);
double Average_Time(int all_times, int car_count);

int main() {
    Car_Route cr[MAX_ROUTES]; /* cr = car route */
    Cars car[MAX_CARS];
    int all_times = 0, i, car_count;
    pthread_t carIteration[MAX_CARS];
    Thread_Args args[MAX_CARS];

    Get_Route(cr);
    for(i = 0; i < MAX_CARS; i++){
        car[i] = Create_Car(car, cr);
    }

    for (i = 0; i < MAX_ROUTES; i++){
        printf("Car init: ---------\n");
        *args = Update_Car_Thread(car[i], cr[i], &all_times, i);
        Print_Route_Summary(cr[i], i);
        pthread_create(&carIteration[i], NULL, Run_Car, args); 
        pthread_join(carIteration[i], NULL);

    }
    car_count = i;
    printf("Sum of time spent: %d \n", all_times);
    printf("Average time of all cars: %lf", Average_Time(all_times, car_count));
    
    return EXIT_SUCCESS;
}

Thread_Args Update_Car_Thread(Cars car, Car_Route cr, int *time, int i) {
    Thread_Args* _arg = malloc(sizeof(*_arg));
    _arg -> car = car;
    _arg -> cr = cr;
    _arg -> time = time;
    _arg -> i = i;
    return *_arg;
}

Cars Create_Car(Cars *car, Car_Route *cr) {
    car -> current_position = cr  ->  start_position;
    car -> start_time = 0;
    car -> route = 1;
    car -> driving_direction = 2;
    return *car;
}

void *Run_Car(void *_args) {
    int time = 0;
    struct Thread_Args *args = (struct Thread_Args *) _args;
    do{
        //printf("Driving direction %d ", args->car -> driving_direction);

        args->car.current_speed = MAX_SPEED;
        args->car.current_position = args->car.current_position + args->car.current_speed;
        //printf("Position: %lf time: %lf\n", args->car -> current_position,time);
        time++;
    }while(Car_Turning(args->car, time, args->cr, args->time, args -> i) != 1);
    return NULL;
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
}

void Print_Route_Summary(Car_Route cr, int i) {
    char* direction;

    printf("Route number = %d\n", cr.route_number);
    printf("Starting position = %d\n", cr.start_position);
    for(i = 0; i < 5; i++) {
        if(cr.intersections[i] == OutOfSystem) 
            break;

        switch (cr.intersections[i]) {
            case 0: direction = "North"; break;
            case 1: direction = "South"; break;
            case 2: direction = "East"; break;
            case 3: direction = "West"; break;
        }
        printf("In intersection %d go %s\n", i, direction);
    }
}

int Car_Turning(Cars car, int time, Car_Route cr, int *all_times, int i) { 

    /*
        TO BE REPLACED LATER
    */


    if(car.current_position >= 100 && car.current_position <= 125) {
        car.driving_direction = cr.intersections[0];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time - car.start_time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if(car.current_position >= 200 && car.current_position <= 225) {
        car.driving_direction = cr.intersections[1];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time - car.start_time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if(car.current_position >= 300 && car.current_position <= 325) {
        car.driving_direction = cr.intersections[2];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time - car.start_time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if(car.current_position >= 400 && car.current_position <= 425) {
        car.driving_direction = cr.intersections[3];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time - car.start_time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if(car.current_position >= 500 && car.current_position <= 525) {
        car.driving_direction = cr.intersections[4];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time - car.start_time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if (car.current_position >= 800) {
        printf("Car finished at time: %d \n", time - car.start_time);
        *all_times += time;
        printf("Done with route, out of system.\n");
        printf("Route end: --------- \n\n");
        return 1;
        pthread_exit(NULL);
    }
    
    return 0;
}

double Average_Time(int all_times, int car_count) {
    return all_times / car_count;
}