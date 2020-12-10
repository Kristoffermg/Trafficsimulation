#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STRING_LENGTH 1000
#define MAX_SPEED 14 /* 50km/t in m/s */
#define CAR_LENGTH 4 /* meters */
#define MAX_CARS 100
#define MAX_ROUTES 4
#define MAX_INTERSECTIONS 5
#define MAX_TIME_VALUES 5000

typedef struct Cars {
    double current_speed;
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

typedef struct Traffic_Light {
    int color; /* 0 = north and south is green, 1 = east and west is green */
    int time_to_switch;
    int next_color_switch;
} Traffic_Light;

enum direction { North, South, East, West, OutOfSystem };
enum Traffic_Light_Colors { Green, Yellow, Red};

Cars Create_Car(Cars *car, Car_Route *cr);
void Run_Car(Cars car, Car_Route cr, int *all_times);
/*int driving_direction(Cars car, Car_Route cr, int n);*/
void Get_Route(Car_Route *cr);
void Print_Route_Summary(Car_Route cr);
int Car_Turning(Cars car, int time, Car_Route cr, int *all_times);
double Average_Time(int all_times, int car_count);
void Init_Traffic_Lights(Traffic_Light *Traffic_Lights);
void Run_Traffic_Lights(int time, Traffic_Light *Traffic_Lights);
void Traffic_Light_Swap_Color(Traffic_Light *Traffic_Lights, int traffic_light_number);

int main() {
    Car_Route cr[MAX_ROUTES]; /* cr = car route */
    Cars car[MAX_CARS];
    Traffic_Light Traffic_Lights[1];
    int all_times;
    int i;
    int car_count = 0;
    Init_Traffic_Lights(Traffic_Lights);
    /*test af run traffic light */
    int time = 0;
    for (i = 0; i<= 10; i++)
    {
        Run_Traffic_Lights(time, Traffic_Lights);
        printf("Lys 1 farve : %d Lys 2 farve: %d\n",Traffic_Lights[0].color, Traffic_Lights[1].color);
        time = time + 1;
    }
    /*slutning af test */
    Get_Route(cr);
    for(i = 0; i < MAX_CARS; i++){
        car[i] = Create_Car(car, cr);
    }

    for(i = 0; i < MAX_ROUTES; i++) {
       printf("Car init: ---------\n"); 
       Print_Route_Summary(cr[i]);
       Run_Car(car[i], cr[i], &all_times);
    }
    car_count = i;
    printf("Total time for all cars: %d \n", all_times);
    printf("Average time of all cars: %lf", Average_Time(all_times, car_count));

    return EXIT_SUCCESS;
}

Cars Create_Car(Cars *car, Car_Route *cr) {
    car -> current_position = cr -> start_position;
    car -> current_position = 0;
    car -> route = 1;
    car -> driving_direction = 2;
    return *car;
}

void Run_Car(Cars car, Car_Route cr, int *all_times) {
    int time = 0;
    do{
        //printf("Driving direction %d ", car -> driving_direction);
        if(car.current_speed < MAX_SPEED) 
            car.current_speed = MAX_SPEED;
        car.current_position += car.current_speed;
        //printf("Position: %lf time: %lf\n", car -> current_position, time);
        time++;
    }while(Car_Turning(car, time, cr, all_times) != 1);
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

void Print_Route_Summary(Car_Route cr) {
    char* direction;
    int i;

    printf("Route number = %d\n", cr.route_number);
    printf("Starting position = %d\n", cr.start_position);
    for(i = 1; i <= 5; i++) {
        switch (cr.intersections[i-1]) {
            case 0: direction = "North"; break;
            case 1: direction = "South"; break;
            case 2: direction = "East"; break;
            case 3: direction = "West"; break;
            case 4: direction = "Out of system"; break;
        }
        printf("In intersection %d go %s\n", i, direction);
    }
}

int Car_Turning(Cars car, int time, Car_Route cr, int *all_times) {
   if(car.current_position >= 100 && car.current_position <= 125) {
        car.driving_direction = cr.intersections[0];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if(car.current_position >= 200 && car.current_position <= 225) {    
        car.driving_direction = cr.intersections[1];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if(car.current_position >= 300 && car.current_position <= 325) {
        car.driving_direction = cr.intersections[2];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if(car.current_position >= 400 && car.current_position <= 425) {
        car.driving_direction = cr.intersections[3];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if(car.current_position >= 500 && car.current_position <= 525) {
        car.driving_direction = cr.intersections[4];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                printf("Car turns at time: %d \n", time);
                *all_times += time;
                printf("Done with route, out of system.\n");
                printf("Route end: --------- \n\n");
                return 1;
            }
    }
    else if (car.current_position >= 800) {
        printf("Car finished at time: %d \n", time);
        *all_times += time;
        printf("Done with route, out of system.\n");
        printf("Route end: --------- \n\n");
        return 1;
    }
    
    return 0;
}

double Average_Time(int all_times, int car_count) {
    return all_times / car_count;
}
void Init_Traffic_Lights(Traffic_Light *Traffic_Lights){
    Traffic_Lights[0].color = Green;
    Traffic_Lights[0].time_to_switch = 5;
    Traffic_Lights[0].next_color_switch = 5;

    Traffic_Lights[1].color = Red;
    Traffic_Lights[1].time_to_switch = 5;
    Traffic_Lights[1].next_color_switch = 5;

}

void Run_Traffic_Lights(int time, Traffic_Light *Traffic_Lights){
    int i = 0; 
    for (i = 0; i<2; i++) {
        if (time == Traffic_Lights[i].next_color_switch) {
            Traffic_Light_Swap_Color(Traffic_Lights, i);
            Traffic_Lights[i].next_color_switch = time + Traffic_Lights[i].time_to_switch;
        }
    }
}

void Traffic_Light_Swap_Color(Traffic_Light *Traffic_Lights, int traffic_light_number){
    if (Traffic_Lights[traffic_light_number].color == Green)
        Traffic_Lights[traffic_light_number].color = Red;
    else if (Traffic_Lights[traffic_light_number].color == Red)
        Traffic_Lights[traffic_light_number].color = Green;
}