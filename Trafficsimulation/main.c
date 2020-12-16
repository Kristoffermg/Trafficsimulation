#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STRING_LENGTH 1000
#define MAX_SPEED 14 /* 50km/t in m/s */
#define CAR_LENGTH 4 /* meters */
#define MAX_CARS 20000
#define MAX_ROUTES 4
#define MAX_INTERSECTIONS 5
#define MAX_TIME_VALUES 5000
#define SECONDS_PER_HOUR 3600

typedef struct Cars {
    int carID;
    double current_speed;
    int start_time;
    int route;
    int current_position;
    int driving_direction; /* uses enum directions values */
    int active; /* 0 if car haven't entered system, 1 if it's currently in the system, 2 if it finished */
    int car_time;
} Cars;

struct Intersection {
    int is_traffic_light;
    int road_connections; /* 0: northwards, 1: southwards, 2: both */
};

typedef struct Car_Route {
    int route_number;
    int start_position;
    int driving_direction;
    int intersections[MAX_INTERSECTIONS];
} Car_Route;

typedef struct Traffic_Light {
    int color; /* 0 = north and south is green, 1 = east and west is green */
    int time_to_switch_green;
    int time_to_switch_red;
    int next_color_switch;
} Traffic_Light;

enum direction { North, South, East, West, OutOfSystem };
enum Traffic_Light_Colors { Green, Yellow, Red};

int Most_Recent_In_Queue(int inactive_cars_queue[], int *index);
int Empty_Spot_In_Queue(int inactive_cars_queue[]);
Cars Create_Car(Cars *car, Car_Route *cr, int time);
void Run_Car(Cars *car, int *all_times, int i);
void Get_Route(Car_Route *cr);
int Random_Route_Num();
void Print_Route_Summary(Car_Route cr);
int Car_Turning(Cars car, Car_Route cr);
int Average_Time(int all_times, int car_count);
void Init_Traffic_Lights(Traffic_Light *Traffic_Lights);
void Run_Traffic_Lights(int time, Traffic_Light *Traffic_Lights);
void Traffic_Light_Swap_Color(Traffic_Light *Traffic_Lights);
int Collision_Check(Cars car, Cars *all_cars, Traffic_Light *traffic_lights, int car_num, int i, int car_count);
int Calculate_Collision_Status(int current_car_pos, int other_car_pos, int driving_direction);
int Calculate_Speed_Decrease_To_Avoid_Collision(int meters_until_collision, int car_speed);

int main() {
    Car_Route cr[MAX_ROUTES]; /* cr = car route */
    Cars car[350];
    Traffic_Light Traffic_Lights[2];
    int all_times = 0, 
        i, 
        current_time = 0, 
        current_hour = 0,
        car_count = 0,
        total_cars = 0,
        car_count_in_an_hour = 0,
        route_num = 0,
        hour_time = 0,
        active_count = 0,
        most_recent_in_queue = 0,
        empty_spot_index = 0,
        index = 0;
    int inactive_cars_queue[350];

    srand(time(NULL));

    Init_Traffic_Lights(Traffic_Lights);

    Get_Route(cr);

    while(current_time < SECONDS_PER_HOUR * 24) {      
        current_time++;
        Run_Traffic_Lights(current_time, Traffic_Lights);
        if(current_time % SECONDS_PER_HOUR == 0) { 
            printf("[%dh] average: %ds, Car count: %d, Hour time: %d\n", ++current_hour, Average_Time(hour_time, car_count_in_an_hour), car_count_in_an_hour, hour_time); 
            car_count_in_an_hour = 0;
            hour_time = 0;
        }

        if(rand() % 17 == 1) {
            route_num = Random_Route_Num();
            if(car_count < 300) {
                car[car_count] = Create_Car(car, &cr[route_num], current_time);
                car[car_count].carID = car_count;
                car_count++;
            }
            else {
                most_recent_in_queue = Most_Recent_In_Queue(inactive_cars_queue, &index);
                car[most_recent_in_queue] = Create_Car(car, &cr[route_num], current_time);
                car[most_recent_in_queue].carID = most_recent_in_queue;
                inactive_cars_queue[index] = 0;
            }
            car_count_in_an_hour++;
            total_cars++;
        }

        for (i = 0; i < car_count; i++) {
            if(car[i].active == 0) {
                car[i].active = 1;
            }
            if(car[i].active == 1 && Car_Turning(car[i], cr[car[i].route]) != 1) {
                car[i].current_speed += Collision_Check(car[i], car, Traffic_Lights, i, 0, car_count);
                if(car[i].driving_direction == East)
                    car[i].current_position += car[i].current_speed;
                else if(car[i].driving_direction == West)
                    car[i].current_position -= car[i].current_speed;
            }
            else if(car[i].active == 1 && Car_Turning(car[i], cr[car[i].route]) == 1) {
                car[i].active = 2;
                empty_spot_index = Empty_Spot_In_Queue(inactive_cars_queue);
                inactive_cars_queue[empty_spot_index] = car[i].carID;
                car[i].car_time = current_time - car[i].start_time;
                all_times += current_time - car[i].start_time;
                hour_time += current_time - car[i].start_time;
                active_count++;
            }
            /* printf("Current pos: %lf time: %d ID: %d \n", car[i].current_position, time, car[i].carID); */
        }
    }
    /*
    for(i = 0; i < car_count; i++) {
        printf("CarID: %d, Car time: %d, Car active: %d\n", car[i].carID, car[i].car_time, car[i].active);
    }*/
    printf("Total number of cars: %d\n", total_cars - 1);
    printf("Total time for all cars: %ds\n", all_times);
    printf("Average time of all cars: %ds\n", Average_Time(all_times, total_cars));

    return EXIT_SUCCESS;
}

int Most_Recent_In_Queue(int inactive_cars_queue[], int *index) {
    int i;
    for(i = 0; i < 300; i++) {
        if(inactive_cars_queue[i] != 0) {
            *index = i;
            return inactive_cars_queue[i];
        }
    }
    return -1;
}

int Empty_Spot_In_Queue(int inactive_cars_queue[]) {
    int i;
    for(i = 0; i < 300; i++) {
        if(inactive_cars_queue[i] == 0) 
            return i;
    }
}

Cars Create_Car(Cars *car, Car_Route *cr, int time) {
    car->current_position = cr->start_position;
    car->route = cr->route_number;
    car->driving_direction = cr->driving_direction;
    car->active = 0;
    car->current_speed = 0;
    car->start_time = time;
    return *car;
}

void Get_Route(Car_Route *cr) {
    int i;

    FILE *routes_file_pointer;
    routes_file_pointer = fopen("Routes.txt", "r");

    for(i = 0; i < MAX_ROUTES; i++) {
        fscanf(routes_file_pointer, "%d %d %d %d %d %d %d %d", &cr[i].route_number, &cr[i].start_position, &cr[i].driving_direction,
                                                                &cr[i].intersections[0], &cr[i].intersections[1], 
                                                                &cr[i].intersections[2], &cr[i].intersections[3],
                                                                &cr[i].intersections[4]);
    }
    fclose(routes_file_pointer);
}

int Random_Route_Num() {
    return rand() % MAX_ROUTES; 
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

int Car_Turning(Cars car, Car_Route cr) {
    if(car.current_position >= 100 && car.current_position <= 125) {
        car.driving_direction = cr.intersections[0];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                return 1;
            }
    }
    else if(car.current_position >= 200 && car.current_position <= 225) {    
        car.driving_direction = cr.intersections[1];
            if(car.driving_direction != 2 && car.driving_direction != 3 ) {
                return 1;
            }
    }
    else if(car.current_position >= 300 && car.current_position <= 325) {
        car.driving_direction = cr.intersections[2];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                return 1;
            }
    }
    else if(car.current_position >= 400 && car.current_position <= 425) {
        car.driving_direction = cr.intersections[3];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                return 1;
            }
    }
    else if(car.current_position >= 500 && car.current_position <= 525) {
        car.driving_direction = cr.intersections[4];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                return 1;
            }
    }
    else if (car.current_position >= 800 && car.driving_direction == East) 
        return 1;
    else if(car.current_position <= 0 && car.driving_direction == West)
        return 1;
    return 0;
}

int Average_Time(int all_times, int car_count) {
    return all_times / car_count;
}

void Init_Traffic_Lights(Traffic_Light *Traffic_Lights){
    Traffic_Lights->color = Green;
    Traffic_Lights->time_to_switch_green = 40;
    Traffic_Lights->time_to_switch_red = 20;
    Traffic_Lights->next_color_switch = 5;

    Traffic_Lights++; /* Sets the pointer to the next position in the array */

    Traffic_Lights->color = Green;
    Traffic_Lights->time_to_switch_green = 40;
    Traffic_Lights->time_to_switch_red = 20;
    Traffic_Lights->next_color_switch = 5;
}

void Run_Traffic_Lights(int time, Traffic_Light *Traffic_Lights){
    int i = 0; 
    for (i = 0; i<2; i++) {
        if (time == Traffic_Lights->next_color_switch) {
            if(Traffic_Lights->color == Green)
                Traffic_Lights->next_color_switch = time + Traffic_Lights->time_to_switch_green;
            else if(Traffic_Lights->color == Red)
                Traffic_Lights->next_color_switch = time + Traffic_Lights->time_to_switch_red;
            Traffic_Light_Swap_Color(Traffic_Lights+i);
        }
    }
}

void Traffic_Light_Swap_Color(Traffic_Light *Traffic_Lights){
    if (Traffic_Lights->color == Green)
        Traffic_Lights->color = Red;
    else if (Traffic_Lights->color == Red)
        Traffic_Lights->color = Green;
}

enum Collision_Status { CarsSamePosition, CurrentCarCollides, OtherCarCollides, CollisionIsClose, NoCollision };

int Collision_Check(Cars car, Cars *all_cars, Traffic_Light *traffic_lights, int car_num, int i, int car_count) {
    int current_car_pos = car.current_position,
        other_car_pos,
        collision_status,
        meters_until_collision;

    if(all_cars[i].active == 1 && i != car_num && car.driving_direction == all_cars[i].driving_direction) {
        other_car_pos = all_cars[i].current_position;
        //printf("current: %d other: %d directionc: %d directiono: %d\n", current_car_pos, other_car_pos, car.driving_direction, all_cars[i].driving_direction);
    }
    else if(i < car_count)
        return Collision_Check(car, all_cars, traffic_lights, car_num, i + 1, car_count);
    else if(car.current_position >= 86 && car.current_position <= 108 && traffic_lights->color == Red)
        other_car_pos = 100;
    else if(car.current_position >= 117 && car.current_position <= 139 && traffic_lights->color == Red)
        other_car_pos = 125;
    else if(car.current_position >= 486 && car.current_position <= 508 && traffic_lights->color == Red)
        other_car_pos = 500;
    else if(car.current_position >= 517 && car.current_position <= 539 && traffic_lights->color == Red)
        other_car_pos = 525;
    else return 0;

    collision_status = Calculate_Collision_Status(current_car_pos, other_car_pos, car.driving_direction);
    //printf("----%d-----\n", collision_status);

    if(collision_status == CarsSamePosition) { /* Quick speed boost when cars start on top of each other at the same time */
        /* printf("SAME POSITION: Current_pos: %d other_car: %d \n", current_car_pos, other_car_pos); */
        if(car.current_speed <= 4)
            return 5;
        else if(car.current_speed >= 5 && car.current_speed <= 9) 
            return 3;
        else if(car.current_speed <= 13)
            return 1;
        else return -2;
    }
    else if(collision_status == OtherCarCollides) { /* Drive faster */
        /* printf("FASTER: Current_pos: %d other_car: %d \n", current_car_pos, other_car_pos); */
        if(car.current_speed + 2 <= MAX_SPEED)
            return 2;
        else return -2;
    }
    else if(collision_status == CollisionIsClose) {
        if(car.driving_direction == East)
            meters_until_collision = other_car_pos - current_car_pos;
        else if(car.driving_direction == West)
            meters_until_collision = current_car_pos - other_car_pos;
        /* printf("CLOSE: Current_pos: %d other_car: %d until_collision: %d\n", current_car_pos, other_car_pos, meters_until_collision); */
        return -Calculate_Speed_Decrease_To_Avoid_Collision(meters_until_collision, car.current_speed);
    }
    else if(collision_status == NoCollision) { /* Maintain same speed */
        /* printf("Maintain: Current_pos: %d other_car: %d \n", current_car_pos, other_car_pos); */
        if(i < car_count && car.current_speed > 0)
            return Collision_Check(car, all_cars, traffic_lights, car_num, i + 1, car_count);
    }

    if(car.current_speed <= 5)
        return 5;
    else if(car.current_speed >= 6 && car.current_speed <= 9)
        return 3;
    else if(car.current_speed >= 10 && car.current_speed < 14)
        return 1;
    return 0;
}

int Calculate_Collision_Status(int current_car_pos, int other_car_pos, int driving_direction) { 
    if(driving_direction == East) {
        if(current_car_pos == other_car_pos)
        return CarsSamePosition;
    else if((current_car_pos + 2) >= other_car_pos && (current_car_pos + 2) <= (other_car_pos + 2)) 
        return CurrentCarCollides;
    else if((other_car_pos + 2) >= current_car_pos && (other_car_pos + 2) <= (current_car_pos + 2))
        return OtherCarCollides;
    else if(other_car_pos > (current_car_pos + 2) && other_car_pos < (current_car_pos + 2) + 8)  /* Checks if the other car is between the current car to 10 meters ahead of the current car */
        return CollisionIsClose;
    else
        return NoCollision;
    }
    else if(driving_direction == West) {
        if(current_car_pos == other_car_pos)
            return CarsSamePosition;
        else if((current_car_pos - 2) <= other_car_pos && (current_car_pos - 2) >= (other_car_pos - 2)) 
            return CurrentCarCollides;
        else if((other_car_pos - 2) <= current_car_pos && (other_car_pos - 2) >= (current_car_pos - 2))
            return OtherCarCollides;
        else if(other_car_pos < (current_car_pos - 2) && other_car_pos > (current_car_pos - 2) - 8)  /* Checks if the other car is between the current car to 10 meters ahead of the current car */
            return CollisionIsClose;
        else
            return NoCollision;
    }
    return -1;
}

int Calculate_Speed_Decrease_To_Avoid_Collision(int meters_until_collision, int car_speed) {
    if(meters_until_collision <= 2) 
        return car_speed;
    else if(meters_until_collision >= 3 && meters_until_collision <= 6) {
        if(car_speed >= 3)
            return 3;
        else return -1;
    }
    else if(meters_until_collision >= 7 && meters_until_collision <= 10) {
        if(car_speed >= 6)
            return 6;
        else return -2;
    }
    return 0;
}