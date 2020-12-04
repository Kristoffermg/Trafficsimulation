#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STRING_LENGTH 1000
#define MAX_SPEED 14 /* 50km/t i m/s */
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


enum direction { North, South, East, West, OutOfSystem };

Cars Create_Car(Cars *car, Car_Route *cr);

int Run_Car(Cars *car, double time, Car_Route *cr, int *All_Times);
//int driving_direction(Cars car, Car_Route cr, int n);
void get_route(Car_Route *cr);
int car_turning(Cars *car, double time, Car_Route *cr, int *All_Times);
void Return_Time(int Car_Time, int *All_Times);
int Int_Convert(char *temp_intersections);

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

    printf("Koersel af bil: ---------\n");
    while (Run_Car(&car[1], time, cr, All_Times) != 1) {
        time++;
    }
    
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
    char c;
    char route[MAX_STRING_LENGTH];
    char t_route[2];
    char t_startpos[2];
    char t_int1[2]; char t_int2[2]; char t_int3[2]; char t_int4[2]; char t_int5[2];
    int i;
    int j;

    FILE *routes_file_pointer;
    routes_file_pointer = fopen("Routes.txt", "r");

    for(i = 0; i < MAX_ROUTES; i++) {
        fgets(route, MAX_STRING_LENGTH, routes_file_pointer);
        //printf("%s\n", route);
        sscanf(route," %1s %1s %1s %1s %1s %1s %1s", t_route, t_startpos, t_int1, t_int2, t_int3, t_int4, t_int5);   

        cr[i].route_number = Int_Convert(t_route);
        cr[i].start_position = Int_Convert(t_startpos);
        cr[i].intersections[0] = Int_Convert(t_int1);
        cr[i].intersections[1] = Int_Convert(t_int2);
        cr[i].intersections[2] = Int_Convert(t_int3);
        cr[i].intersections[3] = Int_Convert(t_int4);
        cr[i].intersections[4] = Int_Convert(t_int5);
    }
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
        printf("In intersection %d Go %s\n",i+1 , direction);
    }
    printf("Done with route, out of system.\n");
    printf("Route end: --------- \n\n");
}

int car_turning(Cars *car, double time, Car_Route *cr, int *All_Times){
    if(car->current_position > 100 && car->current_position < 125) {
        car->driving_direction = cr[0].intersections[0];
    }
    else if(car->current_position > 200 && car->current_position < 225) {
        car->driving_direction = cr[0].intersections[1];
            if(car->driving_direction != 2) {
                printf("bil drejer tid: %lf", time - car->start_time);
                return 1;
            }
    }
    else if (car->current_position > 800) {
        printf("bil færdig tid: %lf", time - car->start_time);
        return 1;
    }
    else
        return 0;
}

void Return_Time(int Car_Time, int *All_Times)
{
    /*Static siden counteren skal tælle 1 op hver gang funktionen kaldes */
    static int Car_Count = 0;
    All_Times[Car_Count] = Car_Time;
    Car_Count++;

    
}

int Int_Convert(char *temp_intersections) {
    return atoi(temp_intersections);   
}