#include <stdio.h>
#include "vector_int.h"
#include "map.h"
#include <string.h>
#include <zconf.h>
#include "priority_queue.h"
#include "track.h"
#include "road.h"
#include <assert.h>

int main() {

   printf("Hello, World!\n");
   Map * mapa = newMap();
   addRoad(mapa, "A", "B", 5, 1);
   addRoad(mapa, "A", "C", 12, 1);
   addRoad(mapa, "A", "D", 10, 1);
   addRoad(mapa, "B", "D", 3, 1);
   addRoad(mapa, "D", "E", 7, 1);
   addRoad(mapa, "E", "F", 1, 1);
   addRoad(mapa, "C", "F", 7, 1);
   bool czy = newRoute(mapa, 1, "D", "A");
   route *  trasa = mapa->routes_list[1];
   extendRoute(mapa, 1, "F");
   printf("moja rozszerzona trasa:\n");
   for(int i = 0;i < mapa->routes_list[1]->parts->length;i++){
      printf("%d ", mapa->routes_list[1]->parts->arr[i]->city1_id);
      printf("%d\n", mapa->routes_list[1]->parts->arr[i]->city2_id);
   }

   vector_city * na_trasie = cities_on_route(mapa, 1);
   vector_city * miasta = new_vector_city();
   city * startowe = mapa->cities_list->arr[0];
   city * miastoB = get_city(1, mapa->cities_list);
   add_city_to_vec(miasta, miastoB);
   track ** wynik = dijkstra(mapa, miasta, startowe);
   // wypisz optymalne trasy
   for(int i = 0;i < mapa->cities_list->length;i++){
      printf("%d ", wynik[i]->year_of_oldest_part);
      printf("%d\n", wynik[i]->length);
   }

}