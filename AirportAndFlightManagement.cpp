#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>

using namespace std; 

//Represents a passenger with a passport number and name.
class Passenger {
public:
    double passportNo;
    char name[50];
    Passenger* next;
    Passenger* prev;
    Passenger(double pNo, const char* n);
    void displayPassenger();
};
//Represents a queue of passengers.
Passenger::Passenger(double pNo, const char* n) {
    passportNo = pNo;
    strcpy(name, n);
    next = NULL;
    prev = NULL;
}

void Passenger::displayPassenger() {
    cout << "Passport No: " << passportNo << endl;
    cout << "Passenger Name: " << name << endl;
}

class PassengerQueue {
private:
    double LastPassengerID = 0;

public:
    Passenger* Front;
    Passenger* Rear;
    PassengerQueue();
    ~PassengerQueue();
    void enqueue(double passportNo, const char* name);
    Passenger* dequeue();
    void displayPassengers();
};

PassengerQueue::PassengerQueue() {
    Front = NULL;
    Rear = NULL;
}
void PassengerQueue::displayPassengers(){
      Passenger* current = Rear;
    while (current != NULL) {
        current->displayPassenger();
        current = current->prev;
    }
}
PassengerQueue::~PassengerQueue() {

}

void PassengerQueue::enqueue(double passportNo, const char* name) {
    if (Front == NULL) {
        Passenger* P1 = new Passenger(passportNo, name);
        P1->next = Front;
        Front = P1;
        Rear = P1;
    } else {
        LastPassengerID++;
        Passenger* P1 = new Passenger(passportNo, name);
        Rear->next = P1;
        Rear = P1;
    }
}

Passenger* PassengerQueue::dequeue() {
    if (Front == NULL) {
        cout << "Queue is empty. Cannot dequeue." << endl;
        return NULL;
    } else {
        Passenger* temp = Front;
        Front = Front->next;
        if (Front == NULL) {
            Rear = NULL;
        }
        return temp;
    }
}
//Represents a flight between two destinations.
class Flight {
public:
    int flightNum;
    Flight* next;
    Flight* previous;
    char start[20];
    char end[20];
    int distance;

    PassengerQueue* seated;
    int seatNum;
    int seatLimit;

    Flight(int number, const char* strt, const char* endd, int seatlimit);
    void displayFlight();
    void addPassengers(Passenger *p);
    void displayPassengers();
};
void Flight::displayPassengers(){
    seated->displayPassengers();
}
Flight::Flight(int number, const char* strt, const char* endd, int seatlimit) : flightNum(number), next(nullptr), previous(nullptr) {
    strcpy(start, strt);
    strcpy(end, endd);
    seatLimit = seatlimit;
    seatNum = 0;
    seated = new PassengerQueue();
}

void Flight::displayFlight() {
    cout << "Flight Number: " << flightNum << endl;
    cout << "Start: " << start << endl;
    cout << "Destination: " << end << endl;
    cout << "Distance: " << distance << endl;
    cout << "Unoccupied seats: " << seatLimit - seatNum << endl;
}
//Method to add a passenger to the flight.
void Flight::addPassengers(Passenger *psngr) {
       if (seatNum < seatLimit) {
        if (seated->Front == NULL) {
            seated->Front = psngr;
            seated->Rear = psngr;
        } else {
            seated->Rear->next = psngr;
            psngr->prev = seated->Rear; // Set the prev pointer for the new passenger
            seated->Rear = psngr;
        }
        seatNum++;
    } else {
        cout << "Flight is already full. Cannot add more passengers." << endl;
    }
}
//Represents an airport in the network.
class Airport {
public:
    string destination;
    int distance;
    Airport* next;
    Airport(const string& d, int dis);
};

Airport::Airport(const string& d, int dis) : destination(d), distance(dis), next(nullptr) {}


//Represents the airport network management system.
class AirportNetwork {
private:
    unordered_map<string, int> cityIndices;
    static const int MAX_VERTICES = 10;
    int V;
    Airport** adjList;
    vector<string> cityNames;
    Flight* flights;
    int flightCounter;

public:
    AirportNetwork() : V(0), flights(nullptr), flightCounter(1) {
        adjList = new Airport*[MAX_VERTICES];
        for (int i = 0; i < MAX_VERTICES; ++i)
            adjList[i] = nullptr;
    }

    ~AirportNetwork() {
        for (int i = 0; i < V; ++i) {
            Airport* current = adjList[i];
            while (current) {
                Airport* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] adjList;

        Flight* currentFlight = flights;
        while (currentFlight) {
            Flight* nextFlight = currentFlight->next;
            delete currentFlight;
            currentFlight = nextFlight;
        }
    }

    void displayAllFlights();
    void addCity(const string& cityName);
    void addEdge(const string& src, const string& dest, int distance);
    int dijkstra(const string& src, const string& dest);
    void printGraph();
    void insertFlight(const char* start, const char* end, int seatlimt);
    void addPassengerToFlight(int id, Passenger* pas);
    void displayFlightPassengers(int id);
};

void AirportNetwork::displayFlightPassengers(int id){
    Flight* current = flights;
    bool flightFound = false;

    while (current != nullptr) {
        if (current->flightNum == id) {
            current->displayPassengers();
            flightFound = true;
            break;
        }
        current = current->next;
    }

    if (!flightFound) {
        cout << "Flight with ID " << id << " not found." << endl;
    }
}
void AirportNetwork::addPassengerToFlight(int id, Passenger *pas) {
      Flight* current = flights;
    bool flightFound = false;

    while (current != nullptr) {
        if (current->flightNum == id) {
            current->addPassengers(pas);
            flightFound = true;
            break;
        }
        current = current->next;
    }

    if (!flightFound) {
        cout << "Flight with ID " << id << " not found." << endl;
    }

}
// Method to display information about all flights.
void AirportNetwork::displayAllFlights() {
    cout << "All Flights:" << endl;
    Flight* currentFlight = flights;
    while (currentFlight != nullptr) {
        currentFlight->displayFlight();
        currentFlight = currentFlight->next;
        cout << "" << endl;
    }
}

void AirportNetwork::addCity(const string& cityName) {
    if (V < MAX_VERTICES) {
        cityNames.push_back(cityName);
        cityIndices[cityName] = V;
        V++;
    }
}

void AirportNetwork::addEdge(const string& src, const string& dest, int distance) {
    if (cityIndices.find(src) != cityIndices.end() && cityIndices.find(dest) != cityIndices.end()) {
        int srcIndex = cityIndices[src];
        int destIndex = cityIndices[dest];
        Airport* newNode = new Airport(dest, distance);
        newNode->next = adjList[srcIndex];
        adjList[srcIndex] = newNode;
    }
}

void AirportNetwork::insertFlight(const char* start, const char* end, int seatlimt) {
    Flight* newFlight = new Flight(flightCounter++, start, end, seatlimt);
    newFlight->distance = dijkstra(start, end);

    if (flights == nullptr) {
        flights = newFlight;
    } else {
        Flight* currentFlight = flights;
        while (currentFlight->next != nullptr) {
            currentFlight = currentFlight->next;
        }
        currentFlight->next = newFlight;
        newFlight->previous = currentFlight;
    }
}

int AirportNetwork::dijkstra(const string& src, const string& dest) {
    int* dist = new int[V];
    int* prev = new int[V];
    for (int i = 0; i < V; ++i) {
        dist[i] = numeric_limits<int>::max();
        prev[i] = -1;
    }

    set<int> visitedSet;
    dist[cityIndices[src]] = 0;

    while (!visitedSet.count(cityIndices[dest])) {
        int u = -1;
        for (int v = 0; v < V; ++v) {
            if (visitedSet.find(v) == visitedSet.end() && (u == -1 || dist[v] < dist[u]))
                u = v;
        }

        if (u == -1) break;

        visitedSet.insert(u);

        for (Airport* current = adjList[u]; current; current = current->next) {
            int v = cityIndices[current->destination];
            int weight = current->distance;
            if (visitedSet.find(v) == visitedSet.end() && dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }

    vector<int> path;
    for (int at = cityIndices[dest]; at != -1; at = prev[at])
        path.push_back(at);

    if (dist[cityIndices[dest]] == numeric_limits<int>::max())
        return -999;
    else {
        return dist[cityIndices[dest]];
    }

    delete[] dist;
    delete[] prev;
}

void AirportNetwork::printGraph() {
    cout << "Vertices in the graph:" << endl;
    for (const auto& city : cityNames) {
        cout << city << endl;
    }
}

int main() {
    AirportNetwork network;

    network.addCity("London");
    network.addCity("New York");
    network.addCity("Paris");
    network.addCity("Tokyo");
    network.addCity("Sydney");
    network.addCity("Los Angeles");
    network.addCity("Los Santos");

    network.addEdge("London", "New York", 100);
    network.addEdge("New York", "Paris", 200);
    network.addEdge("London", "Tokyo", 500);
    network.addEdge("Tokyo", "Sydney", 300);
    network.addEdge("New York", "Los Angeles", 400);
    network.addEdge("Paris", "Los Santos", 1000);

    network.insertFlight("New York", "Paris", 300);
    network.insertFlight("London", "Tokyo", 200);
    network.insertFlight("London", "Los Santos", 100);

    //network.displayAllFlights();
    PassengerQueue checkin1;
    PassengerQueue checkin2;

    checkin1.enqueue(123456, "John Doe");
    checkin2.enqueue(789012, "Jane Smith");


    network.addPassengerToFlight(1,checkin1.dequeue());
    network.addPassengerToFlight(2,checkin2.dequeue());

    network.displayFlightPassengers(1);
    network.displayAllFlights();
    network.printGraph();


    return 0;
}
