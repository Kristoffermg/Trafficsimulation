#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_STRING_LENGTH 1000
#define MAX_SPEED 14 /* 50km/t in m/s */
#define CAR_LENGTH 4 /* meters */
#define MAX_CARS 20000
#define MAX_ROUTES 53
#define MAX_INTERSECTIONS 5
#define MAX_TIME_VALUES 5000
#define SECONDS_PER_HOUR 3600

typedef struct Car {
    int carID;
    double current_speed;
    int start_time;
    int route;
    int start_position;
    int current_position;
    int driving_direction; /* uses enum directions values */
    int active; /* 0 if car haven't entered system, 1 if it's currently in the system, 2 if it finished */
    int car_time;
    int is_in_buffer; /* 0 if is not in buffer, 1 if yes*/
    int is_on_sideroad; /* 0 if is not on sideroad, 1 if yes*/
} Car;

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
    int buffer; /* buffer for number of cars which should be sent from another trafficlight */
    int sideroad_buffer; /* buffer for number of cars which should be sent from another trafficlight */
} Traffic_Light;

enum direction { North, South, East, West, OutOfSystem };
enum Traffic_Light_Colors { Green, Yellow, Red};
enum Collision_Status { CarsSamePosition, CurrentCarCollides, OtherCarCollides, CollisionIsClose, NoCollision };
enum bool { false, true};

void Coordination_Between_Traffic_Lights(Car *car, Traffic_Light *traffic_light, int i);
Car Create_Car(Car *car, Car_Route *cr, int time);
int Most_Recent_In_Queue(int inactive_cars_queue[], int *index);
int Empty_Spot_In_Queue(int inactive_cars_queue[]);
void Run_Car(Car *car, int *all_times, int i);
void Get_Route(Car_Route *cr);
int Random_Route_Num();
int chance_per_hour(int current_hour);
int Car_Turning(Car car, Car_Route cr, Traffic_Light *traffic_light);
int Average_Time(int all_times, int car_count);
void Init_Traffic_Lights(Traffic_Light *Traffic_Lights);
void Run_Traffic_Lights(int time, Traffic_Light *Traffic_Lights);
void Traffic_Light_Swap_Color(Traffic_Light *Traffic_Lights);
int Speed_Change_If_Collision(Car car, Car *all_cars, Traffic_Light *traffic_lights, int car_num, int i, int car_count);
int Determine_Collision_Status(int current_car_pos, int other_car_pos, int driving_direction);
int Determine_Speed_Change_To_Avoid_Collision(int meters_until_collision, int car_speed);
int Check_If_Stop_For_Traffic_Light(Car car, Traffic_Light *traffic_lights);

int main() {
    Car_Route cr[MAX_ROUTES]; /* cr = car route */
    Car car[350];
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
        chance_first_hour = 0,
        most_recent_in_queue = 0,
        empty_spot_index = 0,
        cars_to_simulate = 0,
        index = 0;
    int inactive_cars_queue[1000];
    char yes_or_no;
    
    srand(time(NULL));

    printf("Do you want coordinated traffic lights (yes=y, no=n):");
    scanf("%c", &yes_or_no);
    printf("How many cars do you minimum want to simulate:");
    scanf("%d", &cars_to_simulate);
    Init_Traffic_Lights(Traffic_Lights);

    Get_Route(cr);

    chance_first_hour = 70 *(5000.0 / cars_to_simulate);

    while(current_time < SECONDS_PER_HOUR * 24) {     
        current_time++;
        Run_Traffic_Lights(current_time, Traffic_Lights);

        if(rand() % (chance_first_hour / chance_per_hour(current_hour)) == 1) {
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
        }
        for (i = 0; i < car_count; i++) {
            if(car[i].active == 0){ 
                car[i].active = 1;
            }
            if (yes_or_no == 'y') {
                Coordination_Between_Traffic_Lights(car, Traffic_Lights, i);
            }

            if(car[i].active == 1 && Car_Turning(car[i], cr[car[i].route], Traffic_Lights) != 1) {
                car[i].current_speed = Speed_Change_If_Collision(car[i], car, Traffic_Lights, i, 0, car_count);
                if(car[i].driving_direction == East)
                    car[i].current_position += car[i].current_speed;
                else if(car[i].driving_direction == West)
                    car[i].current_position -= car[i].current_speed;
            }
            else if(car[i].active == 1 && Car_Turning(car[i], cr[car[i].route], Traffic_Lights) == 1) {
                car[i].active = 2;
                empty_spot_index = Empty_Spot_In_Queue(inactive_cars_queue);
                inactive_cars_queue[empty_spot_index] = car[i].carID;
                car[i].car_time = current_time - car[i].start_time;
                all_times += current_time - car[i].start_time;
                hour_time += current_time - car[i].start_time;
                active_count++;
                total_cars++;
                car_count_in_an_hour++;
                car[i].is_in_buffer = 0;
            }
        }

        if(current_time % SECONDS_PER_HOUR == 0) { 
            printf("[%dh] average: %ds, Car count: %d, Hour time: %d\n", ++current_hour, Average_Time(hour_time, car_count_in_an_hour), car_count_in_an_hour, hour_time);
            car_count_in_an_hour = 0;
            hour_time = 0;
        }
    }
    printf("Total number of cars: %d\n", total_cars - 1);
    printf("Average number of cars per hour: %d\n", total_cars/24);

    printf("Total time for all cars: %dh %dm %ds - in sec only: %ds\n", all_times / SECONDS_PER_HOUR, (all_times/60) % 60, all_times % 60, all_times);
    printf("Average time of all cars: %ds\n", Average_Time(all_times, total_cars));

    return EXIT_SUCCESS;
}

void Init_Traffic_Lights(Traffic_Light *Traffic_Lights){
    int green_time;
    int red_time;
    printf("Insert amount of time traffic lights should be red or green for main road\n Red: ");
    scanf("%d",&red_time);
    printf("\nGreen: ");
    scanf("%d",&green_time);
    Traffic_Lights->color = Green;
    Traffic_Lights->time_to_switch_green = green_time;
    Traffic_Lights->time_to_switch_red = red_time;
    Traffic_Lights->next_color_switch = 40;
    Traffic_Lights->buffer = 0;
    Traffic_Lights->sideroad_buffer = 0;

    (Traffic_Lights + 1)->color = Green;
    (Traffic_Lights + 1)->time_to_switch_green = green_time;
    (Traffic_Lights + 1)->time_to_switch_red = red_time;
    (Traffic_Lights + 1)->next_color_switch = 40;
    (Traffic_Lights + 1)->buffer = 0;
    (Traffic_Lights + 1)->sideroad_buffer = 0;
}

void Run_Traffic_Lights(int time, Traffic_Light *Traffic_Lights){
    int i = 0; 
    for (i = 0; i<2; i++) {
        if (time == (Traffic_Lights+i)->next_color_switch + ((Traffic_Lights+i)->buffer)*2 -(Traffic_Lights + i)->sideroad_buffer) {
            if((Traffic_Lights+i)->color == Green)
                (Traffic_Lights+i)->next_color_switch = time + (Traffic_Lights+i)->time_to_switch_green;
            else if((Traffic_Lights+i)->color == Red)
                (Traffic_Lights+i)->next_color_switch = time + (Traffic_Lights+i)->time_to_switch_red;
            Traffic_Light_Swap_Color((Traffic_Lights+i));
        }
    }
}

void Coordination_Between_Traffic_Lights(Car *car, Traffic_Light *traffic_light, int i){
    if(car[i].current_position != car[i].start_position){
        if(car[i].current_position >= 50 && car[i].current_position <= 65 && car[i].driving_direction == East){
            if(car[i].is_in_buffer == false){
                traffic_light[1].buffer++;
                car[i].is_in_buffer = true;
            }
        }
        else if(car[i].current_position >= 410 && car[i].current_position <= 425 && car[i].driving_direction == West){
            if(car[i].is_in_buffer == false){
                traffic_light[0].buffer++;
                car[i].is_in_buffer = true;
            }
        }
        else if(car[i].current_position >= 0 && car[i].start_position == 0 && car[i].driving_direction == East){
            if(car[i].is_in_buffer == false){
                car[i].is_on_sideroad = false;
                traffic_light[0].buffer++;
                car[i].is_in_buffer = true;
            }
        }
        else if(car[i].current_position == 475 && car[i].start_position == 475 && car[i].driving_direction == West){
            if(car[i].is_in_buffer == false){
                car[i].is_on_sideroad = false;
                traffic_light[1].buffer++;
                car[i].is_in_buffer = true;
            }
        }

        if(car[i].current_position <= 50 && car[i].driving_direction == West){
            if(car[i].is_in_buffer == true){
                traffic_light[0].buffer--;
                car[i].is_in_buffer = false;
            }
        }
        else if(car[i].current_position >= 425 && car[i].driving_direction == East){
            if(car[i].is_in_buffer == true){
                traffic_light[1].buffer--;
                car[i].is_in_buffer = false;
            }
        }
    }
}

int Car_Turning(Car car, Car_Route cr, Traffic_Light *traffic_light) {
    int prev_direction;
    if(car.current_position >= 50 && car.current_position <= 65) {
        prev_direction = car.driving_direction;
        car.driving_direction = cr.intersections[0];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                if(prev_direction == East && car.is_in_buffer == true){
                    (traffic_light + 1)->buffer--;
                }
                else if(prev_direction == West && car.is_in_buffer == true){
                    traffic_light->buffer--;
                }
                return 1;
            }
    }
    else if(car.current_position >= 128 && car.current_position <= 143) {  
        prev_direction = car.driving_direction;  
        car.driving_direction = cr.intersections[1];
            if(car.driving_direction != 2 && car.driving_direction != 3 ) {
                if(prev_direction == East && car.is_in_buffer == true){
                    (traffic_light + 1)->buffer--;
                }
                else if(prev_direction == West && car.is_in_buffer == true){
                    traffic_light->buffer--;
                }
                return 1;
            }
    }
    else if(car.current_position >= 242 && car.current_position <= 257) {
        prev_direction = car.driving_direction;
        car.driving_direction = cr.intersections[2];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                if(prev_direction == East && car.is_in_buffer == true){
                    (traffic_light + 1)->buffer--;
                }
                else if(prev_direction == West && car.is_in_buffer == true){
                    traffic_light->buffer--;
                }
                return 1;
            }
    }
    else if(car.current_position >= 322 && car.current_position <= 337) {
        prev_direction = car.driving_direction;
        car.driving_direction = cr.intersections[3];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                if(prev_direction == East && car.is_in_buffer == true){
                    (traffic_light + 1)->buffer--;
                }
                else if(prev_direction == West && car.is_in_buffer == true){
                    traffic_light->buffer--;
                }
                return 1;
            }
    }
    else if(car.current_position >= 410 && car.current_position <= 425) {
        prev_direction = car.driving_direction;
        car.driving_direction = cr.intersections[4];
            if(car.driving_direction != 2 && car.driving_direction != 3) {
                if(prev_direction == East && car.is_in_buffer == true){
                    (traffic_light + 1)->buffer--;
                }
                else if(prev_direction == West && car.is_in_buffer == true){
                    traffic_light->buffer--;
                }
                return 1;
            }
    }
    else if (car.current_position >= 475 && car.driving_direction == East)
        return 1;
    else if(car.current_position <= 0 && car.driving_direction == West)
        return 1;
    return 0;
}

int Speed_Change_If_Collision(Car car, Car *all_cars, Traffic_Light *traffic_lights, int car_num, int i, int car_count) {
    int current_car_pos = car.current_position,
        other_car_pos,
        collision_status,
        meters_until_collision;

    if(all_cars[i].active == 1 && i != car_num && car.driving_direction == all_cars[i].driving_direction)
        other_car_pos = all_cars[i].current_position;
    else if(i < car_count)
        return Speed_Change_If_Collision(car, all_cars, traffic_lights, car_num, i + 1, car_count);
    else if(Check_If_Stop_For_Traffic_Light(car, traffic_lights) != 0){
        other_car_pos = Check_If_Stop_For_Traffic_Light(car, traffic_lights);
    }
    else if(Check_If_Stop_For_Traffic_Light(car, traffic_lights + 1) != 0){
        other_car_pos = Check_If_Stop_For_Traffic_Light(car, traffic_lights + 1);
    }
    else return car.current_speed;

    collision_status = Determine_Collision_Status(current_car_pos, other_car_pos, car.driving_direction);

    if(collision_status == CarsSamePosition) { /* Quick speed boost when cars start on top of each other at the same time */
        if(car.current_speed <= 4)
            return car.current_speed + 5;
        else if(car.current_speed >= 5 && car.current_speed <= 9) 
            return car.current_speed + 3;
        else if(car.current_speed <= 13)
            return car.current_speed + 1;
        else return car.current_speed - 2;
    }
    else if(collision_status == OtherCarCollides) { /* Drive faster */
        if(car.current_speed + 2 <= MAX_SPEED)
            return car.current_speed + 2;
        else return car.current_speed - 2;
    }
    else if(collision_status == CollisionIsClose) { /* Increase/decrease speed depending on collision risk */
        if(car.driving_direction == East) 
            meters_until_collision = other_car_pos - current_car_pos;
        else if(car.driving_direction == West)
            meters_until_collision = current_car_pos - other_car_pos;
        return Determine_Speed_Change_To_Avoid_Collision(meters_until_collision, car.current_speed);
    }
    else if(collision_status == NoCollision) { /* Maintain same speed */
        if(i < car_count && car.current_speed > 0)
            return Speed_Change_If_Collision(car, all_cars, traffic_lights, car_num, i + 1, car_count);
        else if(car.current_speed <= 5)
            return car.current_speed + 5;
        else if(car.current_speed >= 6 && car.current_speed <= 9)
            return car.current_speed + 3;
        else if(car.current_speed >= 10 && car.current_speed < 14)
            return car.current_speed + 1;
    }
    return car.current_speed;
}

int Determine_Collision_Status(int current_car_pos, int other_car_pos, int driving_direction) { 
    if(driving_direction == East) {
        if(current_car_pos == other_car_pos)
        return CarsSamePosition;
    else if((current_car_pos + 2) >= other_car_pos && (current_car_pos + 2) <= (other_car_pos + 2)) 
        return CurrentCarCollides;
    else if((other_car_pos + 2) >= current_car_pos && (other_car_pos + 2) <= (current_car_pos + 2))
        return OtherCarCollides;
    else if(other_car_pos > (current_car_pos + 2) && other_car_pos < (current_car_pos + 2) + 15)  /* Checks if the other car is between the current car to 10 meters ahead of the current car */
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
        else if(other_car_pos < (current_car_pos - 2) && other_car_pos > (current_car_pos - 2) - 15)  /* Checks if the other car is between the current car to 10 meters ahead of the current car */
            return CollisionIsClose;
        else
            return NoCollision;
    }
    return -1;
}

int Determine_Speed_Change_To_Avoid_Collision(int meters_until_collision, int car_speed) {
    if(meters_until_collision <= 2) 
        return car_speed;
    else if(meters_until_collision >= 3 && meters_until_collision <= 6) {
        if(car_speed >= 2)
            return car_speed - (car_speed - 2);
        else return car_speed + 1;
    }
    else if(meters_until_collision >= 7 && meters_until_collision <= 10) {
        if(car_speed >= 6)
            return car_speed - (car_speed - 6);
        else return car_speed + 2;
    }
    else if(meters_until_collision >= 11 && meters_until_collision <= 15) {
        if(car_speed >= 12) 
            return car_speed - (car_speed - 12);
        else return car_speed + 5;
    }
    return 0;
}

int Check_If_Stop_For_Traffic_Light(Car car, Traffic_Light *traffic_lights){
    if(car.current_position >= 40 && car.current_position <= 57 && traffic_lights->color == Red)
        return 50;
    else if(car.current_position >= 58 && car.current_position <= 75 && traffic_lights->color == Red)
        return 65;
    else if(car.current_position >= 400 && car.current_position <= 417 && traffic_lights->color == Red)
        return 410;
    else if(car.current_position >= 418 && car.current_position <= 435 && traffic_lights->color == Red)
        return 425;
    return 0;
}

Car Create_Car(Car *car, Car_Route *cr, int time) {
    car->current_position = cr->start_position;
    car->start_position = cr->start_position;
    car->route = cr->route_number;
    car->driving_direction = cr->driving_direction;
    car->active = 0;
    car->current_speed = 0;
    car->start_time = time;
    car->is_in_buffer = false;
    car->is_on_sideroad = false;
    return *car;
}

int Most_Recent_In_Queue(int inactive_cars_queue[], int *index) {
    int i;
    for(i = 0; i < 950; i++) {
        if(inactive_cars_queue[i] != 0) {
            *index = i;
            return inactive_cars_queue[i];
        }
    }
    return -1;
}

int Empty_Spot_In_Queue(int inactive_cars_queue[]) {
    int i;
    for(i = 0; i < 950; i++) {
        if(inactive_cars_queue[i] == 0) 
            return i;
    }
}

void Traffic_Light_Swap_Color(Traffic_Light *Traffic_Lights){
    if (Traffic_Lights->color == Green)
        Traffic_Lights->color = Red;
    else if (Traffic_Lights->color == Red)
        Traffic_Lights->color = Green;
}

int chance_per_hour(int current_hour) {
    switch(current_hour) {
        case 0: return 1;
        case 1: return 1;
        case 2: return 1;
        case 3: return 1;
        case 4: return 1;
        case 5: return 2;
        case 6: return 4;
        case 7: return 9;
        case 8: return 6;
        case 9: return 5;
        case 10: return 5;
        case 11: return 5;
        case 12: return 5;
        case 13: return 6;
        case 14: return 6;
        case 15: return 9;
        case 16: return 8;
        case 17: return 6;
        case 18: return 4;
        case 19: return 3;
        case 20: return 2;
        case 21: return 2;
        case 22: return 2;
        case 23: return 1;
    }
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

int Average_Time(int all_times, int car_count) {
    return all_times / car_count;
}
