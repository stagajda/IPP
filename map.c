#include <stdlib.h>
#include "map.h"
#include "vector_road.h"
#include "vector_city.h"
#include "vector_int.h"
#include "road.h"
#include "track.h"
#include "priority_queue.h"
#include <string.h>
#include <zconf.h>
#include <stdio.h>

Map* newMap(){
   Map* new_map = malloc(sizeof(Map));
   new_map->cities_list = new_vector_city();
   new_map->roads_list = new_vector_road();
   new_map->routes_list = malloc(sizeof(route *) * 1000);
   return new_map;
}

bool is_route_part(Map * map, unsigned routeId, city * start_city){
   vector_road * route_parts = map->routes_list[routeId]->parts;
   road * part;
   for(int i = 0;i < route_parts->length;i++){
      part = get_road(i, route_parts);
      if(part->city1_id == start_city->id || part->city2_id == start_city->id){
         return true;
      }
   }
   return false;
}

// funkcja mowi, czy na drodze krajowej route1 miasto city1 wystepuje
// wczesniej niz miasto city2
// zakladamy, ze city1 i city2 sa rozne i naleza do tej drogi krajowej
bool is_earlier_on_route(city * city1, city * city2, route * route1){
   // jezeli jedno z nich jest koncem lub poczatkiem to wiadomo:
   if(route1->city1 == city1 || route1->city2 == city2){
      return true;
   }
   if(route1->city1 == city2 || route1->city2 == city1){
      return false;
   }
   road * road1;
   for(int i = 0;i < route1->parts->length;i++){
      road1 = get_road(i, route1->parts);
      if(road1->city1_id == city1->id || road1->city2_id == city1->id){
         return true;
      }
      if(road1->city1_id == city2->id || road1->city2_id == city2->id){
         return true;
      }
   }
}

// zwraca liste miast, ktore naleza do drogi o danym identyfikatorze
// miasta powtarzaja sie w wynikowym wektorze
vector_city * cities_on_route(Map * map, unsigned route_id){
   route * route1 = map->routes_list[route_id];
   vector_city * result = new_vector_city();
   road * part;

   for(int i = 0;i < route1->parts->length;i++){
      part = get_road(i, route1->parts);
      add_city_to_vec(result, get_city(part->city1_id, map->cities_list));
      add_city_to_vec(result, get_city(part->city2_id, map->cities_list));
   }
   return result;
}

// zwraca miasto o podanej nazwie, lub NULL, jesli nie istnieje
city * find_city(Map * mapa, const char *city){
   for(int i = 0;i < mapa->cities_list->length;i++){
      if(strcmp(get_city(i, mapa->cities_list)->name, city) == 0){
         return get_city(i, mapa->cities_list);
      }
   }
   return NULL;
}
// jezeli jest droga pomiedzy city1 i city2, to ja zwraca,
// w przeciwnym razie NULL, w szczegolnosci, gdy jedno z miast nie istnieje
// to zwraca NULL
road * road_between(city * city1, city * city2){

   if(city1 == NULL || city2 == NULL){
      return NULL;
   }
   for(int i = 0;i < city1->roads->length;i++){
      road * road1 = get_road(i, city1->roads);
      if(road1->city1_id == city1->id || road1->city2_id == city2->id){
         return road1;
      }
   }
   return NULL;
}

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje je do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */

bool addRoad(Map *map, const char *city_name1, const char *city_name2,
             unsigned length, int builtYear) {

   if(strcmp(city_name1, city_name2) == 0) {
      return false;
   }

   city * city1 = find_city(map, city_name1);
   city * city2 = find_city(map, city_name2);
   if(road_between(city1, city2) != NULL){
      return false;
   }
   if(city1 == NULL){
      city1 = add_city_to_map(map, city_name1);
   }
   if(city2 == NULL){
      city2 = add_city_to_map(map, city_name2);
   }

   road * new_road1 = new_road(city1->id, city2->id, length, builtYear);
   add_road_to_vec(city1->roads, new_road1);
   add_road_to_vec(city2->roads, new_road1);
   add_road_to_vec(map->roads_list, new_road1);
}

// przyjmuje nazwe miasta, funkcja zaklada, ze danego miasta nie bylo wczesniej na mapie
// dodaje je do mapy i zwraca jego id
city * add_city_to_map(Map* map,const char *city_name){
   vector_city * list_of_cities = map->cities_list;
   city * created_city = new_city(city_name, list_of_cities->length);
   add_city_to_vec(list_of_cities, created_city);
   return created_city;
}

/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *city_name1, const char *city_name2, int repairYear){
   city * city1 = find_city(map, city_name1);
   city * city2 = find_city(map, city_name2);
   road * road1 = road_between(city1, city2);
   if(road1 == NULL){
      return false;
   }
   if(repairYear == 0 || repairYear < road1->built_year){
      return false;
   }
   road1->built_year = repairYear;

   return true;
}

// znajduje najkrotsza trase do miast na mapie, wszystkie te trasy nie przechodza
// przez miasta nalezace do wektora ignored_cities
// miasta nalezace do ignored_cities moga sie powtarzac
track ** dijkstra(Map * map, vector_city * ignored_cities,  city * start_city){
   track ** track_to;
   int cities_num = map->cities_list->length;
   track_to = malloc(cities_num * sizeof(track *));
   // na poczatku do wszystkich ustawiamy maksymalna odleglosc
   for(int i = 0;i < cities_num;i++){
      track_to[i] = new_track(i);
      track_to[i]->length = INT_MAX;
   }
   // teraz tym ignorowanym ustawiam taka mala odleglosc, ze nigdy nie poprawie w nich wyniku dijkstra
   // wiec nigdy nie bede bral pod uwage krawedzi z nich wychodzacych
   for(int i = 0;i < ignored_cities->length;i++){
      track_to[ignored_cities->arr[i]->id]->length = 0;
   }

   track_to[start_city->id]->length = 0;
   track_to[start_city->id]->year_of_oldest_part = INT_MAX;

   // wrzucam moj startowy wierzcholek na kolejke
   priority_queue * queue = new_priority_queue();
   add_to_queue(queue, track_to[start_city->id]);

   // dopoki cos jest na kolejce
   // bierzemy trase ze szczytu kolejki, jezeli odleglosc i rok sa aktualne, to rozpatrujemy wszystkie krawedzie
   // z koncowego miasta i jezeli jakas krawedz poprawia wynik dla ktoregos miasta, to rozszerzamy trase o nia
   // poprawiamy wynik w tablicy i wrzucamy na kolejke
   while(!is_empty(queue)){
      track * track1 = pop_best(queue);
      int dest = track1->destination_id;
      city * dest_city = get_city(dest, map->cities_list);
      if(track1->length == track_to[dest]->length &&
         track1->year_of_oldest_part == track_to[dest]->year_of_oldest_part){
         for(int i = 0;i < dest_city->roads->length;i++){
            road * road1 = get_road(i, dest_city->roads);
            track * extended_track = extend_track(track1, road1);
            if(is_better_track(extended_track, track_to[extended_track->destination_id]) > 0){
               track_to[extended_track->destination_id] = extended_track;
               add_to_queue(queue, extended_track);
            }
            else{
               // zwalniamy pamiec z tego rozszerzonego tracku
            }
         }
      }
   }
   return track_to;
}
// funkcja przyjmuje dwa miasta city1, city2 i tablice z najbardziej optymalnymi trasami z city1 do danego miasta
// funkcja zwraca wektor z odcinkami ukladajacymi sie w najbardziej optymalna
// droge z city1 do city2 jezeli taka droga nie istnieje, to zwraca NULL
vector_road * best_parts(city * city1, city * city2, track ** tracks){
   if(tracks[city2->id]->length == INT_MAX){
      return NULL;
   }
   vector_road * result = new_vector_road();
   road * part;
   int end_id = city2->id;
   int previous_city_id;
   part = tracks[end_id]->last_road;
   while(true){
      if(part->city1_id == end_id){
         previous_city_id = part->city2_id;
      }
      else{
         previous_city_id = part->city1_id;
      }
      add_road_to_vec(result, part);
      if(previous_city_id == city1->id){
         break;
      }
      else{
         end_id = previous_city_id;
         part = tracks[end_id]->last_road;
      }
   }
   reverse(result);
   return result;
}

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId,
              const char *city1_name, const char *city2_name){
   if(routeId > 999 || strcmp(city1_name, city2_name) == 0){
      return false;
   }
   if(map->routes_list[routeId] != NULL){
      return false;
   }

   city * city1 = find_city(map, city1_name);
   city * city2 = find_city(map, city2_name);
   if(city1 == NULL || city2 == NULL){
      return false;
   }
   route * new_route1 = new_route(routeId, city1, city2);
   vector_city * no_ignored = new_vector_city();
   // znajdujemy kolejne odcinki
   track ** tracks = dijkstra(map, no_ignored, city1);
   new_route1->parts = best_parts(city1, city2, tracks);
   if(new_route1->parts == NULL){
      return NULL;
   }
   road * part;
   // wszystkim tym odcinkom, ktore naleza do tej drogi krajowej wpisujemy ta informacje
   for(int i = 0;i < new_route1->parts->length;i++){
      part = get_road(i, new_route1->parts);
      add(part->routes, new_route1->id);
   }
   map->routes_list[routeId] = new_route1;
   return true;
}


/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
 // TESTOWAC TO
bool extendRoute(Map *map, unsigned routeId, const char *city_name){
   city * start_city = find_city(map, city_name);
   route * route1 = map->routes_list[routeId];
   if(route1 == NULL || start_city == NULL){
      return false;
   }
   if(is_route_part(map, routeId, start_city)){
      return false;
   }
   vector_city * ignored_cities = cities_on_route(map, routeId);
   // przez te na koncach route1 moze dijkstra przechodzic;
   vector_city * ignored_cities1 = new_vector_city();
   for(int i = 0;i < ignored_cities->length;i++){
      city * city1 = get_city(i, ignored_cities);
      if(city1 != route1->city1 && city1 != route1->city2){
         add_city_to_vec(ignored_cities1, city1);
      }
   }

   track ** tracks = dijkstra(map, ignored_cities1, start_city);
   city * route_begin = map->routes_list[routeId]->city1;
   city * route_end = map->routes_list[routeId]->city2;
   int comparision = is_better_track(tracks[route_begin->id], tracks[route_end->id]);
   vector_road * extension;
   if(comparision > 0){
      // przedluzamy trase do postaci start_city -> route_begin -> route_end
      extension = best_parts(start_city, route_begin, tracks);

      road * part;
      // tym z rozszerzenia dodajemy informacje, ze od teraz naleza do tej drogi krajowej
      for(int i = 0;i < extension->length;i++){
         part = get_road(i, extension);
         add(part->routes, routeId);
      }
      route1->parts = append(extension, route1->parts);
      route1->city1 = start_city;

   }
   else{
      if(comparision == 0) {
         // wybor nie jest jednoznaczny, wiec przerywamy
         return false;
      }
      // te tutaj sa w odwrotnej kolejnosci
      extension = best_parts(start_city, route_end, tracks);
      reverse(extension);
      route1->parts = append(route1->parts, extension);
      route1->city2 = start_city;
      // przedluzamy trase do postaci route_begin->route_end->start_city
   }
   return true;
   // szukamy optymalnych drog z city do city1 i city2
}

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map *map, const char *city1_name, const char *city2_name){
   city * city1 = find_city(map, city1_name);
   city * city2 = find_city(map, city2_name);
   if(city1 == NULL || city2 == NULL){
      return false;
   }
   road * deleted_road = road_between(city1, city2);
   if(deleted_road == NULL){
      return false;
   }
   delete_road(city1->roads, deleted_road);
   delete_road(city2->roads, deleted_road);
   for(int i = 0;i < deleted_road->routes->length;i++){
      route * route1 = map->routes_list[get(i, deleted_road->routes)];
      vector_road * parts1 = new_vector_road();
      vector_road * parts2 = new_vector_road();
      bool second_part = false;
      // parts1 to te przed usuwana droga, parts2 po
      for(int i = 0;i < route1->parts->length;i++){
         if(get_road(i, route1->parts) == deleted_road){
            second_part = true;
         }
         else{
            if(!second_part){
               add_road_to_vec(parts1, get_road(i, route1->parts));
            }
            else{
               add_road_to_vec(parts2, get_road(i, route1->parts));
            }
         }
      }
      // ustalam tak, ze city1 bylo wczesniej niz city2
      if(!is_earlier_on_route(city1, city2, route1)){
         city * temp = city2;
         city2 = city1;
         city1 = temp;
      }
      // teraz szukamy sciezki z city1 do city2
      // ignorowane miasta to te co na route oprocz city1 i city2
      vector_city * on_route_cities = cities_on_route(map, route1->id);
      vector_city * ignored_cities = new_vector_city();
      for(int i = 0;i < on_route_cities->length;i++){
         if(get_city(i, on_route_cities) != city1 && get_city(i, on_route_cities) != city2){
            add_city_to_vec(ignored_cities, get_city(i, on_route_cities));
         }
      }
      // teraz dijkstra szukamy sciezki z city1 do city2
      track ** tracks = dijkstra(map, ignored_cities, city1);
      vector_road * best_path = best_parts(city1, city2, ignored_cities);
      if(best_path == NULL){
         return false;
      }
      // teraz tym z best_path dopisujemy, ze sie znajda na tym route
      for(int i = 0;i < best_path->length;i++){
         add(get_road(i, best_path)->routes, route1->id);
      }
      // zmieniamy odcinki routa
      route1->parts = append(append(parts1,best_path), parts2);
   }
   return true;
}
