#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STRING_LENGTH 1000
#define MAX_SPEED 14 /* 50 km/t in m/s */
#define CAR_LENGTH 4 /* meters */
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

/* Alt over denne kommentar blev copy pasted ind*/

/* Der skal tilføjes at hvis lyset vandret er grønt, skal lodret være rødt vice versa*/
    /* Done, tror jeg.*/
/* Mekanismen som den tæller biler op med skal anvendes, ved at aflæse arrayet som er inden lyskrydset*/
/* pga hvordan trafiklys virker irl bør der helst kun kunne indlæses maks 3-4 biler i køen foran lyskrydset */

/* Der skal være 2 trafiklys, fremfor ét*/

/* Sammenordningen som opprioriterer 2 grønne lys i streg skal også implementeres*/

void Red_Or_Green(char trafficLight1NorthSouth, char trafficLight1EastWest, char trafficLight2NorthSouth, char trafficLight2EastWest,
                     char green, char yellow, char red);
    /* There should be a short period in which both directions have a red light to prevent accidents*/
    const double green = 0, yellow = 1, red = 2;
    char trafficLight1EastWest[3];
         trafficLight1EastWest[0] = green;
         trafficLight1EastWest[1] = yellow;
         trafficLight1EastWest[2] = red;

    char trafficLight1NorthSouth[3];
         trafficLight1NorthSouth[0] = green;
         trafficLight1NorthSouth[1] = yellow; 
         trafficLight1NorthSouth[1] = red; 
/* Ville dette gøre sådan er der er to trafiklys?
    char trafficLight2EastWest[3];
         trafficLight2EastWest[0] = green;
         trafficLight2EastWest[1] = yellow;
         trafficLight2EastWest[2] = red;

    char trafficLight2NorthSouth[3];
         trafficLight2NorthSouth[0] = green;
         trafficLight2NorthSouth[1] = yellow;
         trafficLight2NorthSouth[2] = red;     
*/
    if ( trafficLight1EastWest == 0){    /* if EastWest is green then NorthSouth is red*/
        trafficLight1NorthSouth = 2;
    }
        else{
        trafficLight1NorthSouth = 0;         /* if EastWest is anything but green (aka red) */
        }                                   /* then EastWest is green */

//  if ( trafficLight2EastWest == 0){    /* if EastWest is green then NorthSouth is red*/
//      trafficLight2NorthSouth = 2;
//  }
//      else{
//      trafficLight2NorthSouth = 0;         /* if EastWest is anything but green (aka red) */
//      }                                   /* then EastWest is green */

/*  When a traffic light gives a green light to one directon, the other direction should start counting
    up to when it gets to have the green light, every time a car passes the stop line at the traffic light
    the green light should be extended, until a certain amount (such has 60 seconds)    */
void Count_To_Green(/* Placeholder */)

trafficLightEastWest = 0;   /* The traffic light EastWest starts as green */
                            /* Because one has to start green */

void Green_Wave(/*Placeholder*/)
/*
int main();{

}
*/
