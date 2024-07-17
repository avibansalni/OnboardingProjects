#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <functional>
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <chrono>
#include <set>

#include <chrono>
#include <functional>

class Rooms{
    public:
        std::string name;
        struct tm* time;
        int capacity;
        bool available;
        Rooms(){
            name = "";
            time_t tt = std::time(time_t());
	        time = (localtime(&tt));
            capacity = 0;
            available = true;
        }
        Rooms(std::string Name, int capacity){
            this->name = Name;
            time_t tt = std::time(time_t());
	        this->time = (localtime(&tt));
            this->capacity = capacity;
            this->available = true;
        }

        bool operator<(const Rooms& room) const {
            return this->capacity < room.capacity;
        }
        
        bool operator>(const Rooms& room) const {
            return this->capacity > room.capacity;
        }
};

class TaskClass {
    public:
        std::function<void()> func;
        Rooms room;
        TaskClass(std::function<void()> f, Rooms room, int time){
            this->func = f;
            this->room = room;
            this->room.available = false;
            this->room.time->tm_min = time;
        };
};

class Compare {
    public:
        bool operator()(TaskClass t1, TaskClass t2) {
            return std::difftime(std::mktime(t1.room.time),std::mktime(t2.room.time)) >= 0;
        }
};

class comp {
    public:
        bool operator()(std::pair<Rooms, int> l, std::pair<Rooms, int> r) const
        {
            return l.second <= r.second;
        }
};

class RoomsQueue{
    private:
        std::vector<std::thread> threads;
        std::priority_queue<TaskClass, std::vector<TaskClass>, Compare> tasks;
        std::mutex m;
        std::condition_variable cv;
        std::map<Rooms, int> roomsFree;
        std::map<Rooms, int> roomsBooked;

    public:
        RoomsQueue(int numberOfRooms){
            for(int i = 0; i < numberOfRooms; i++)
            {
                threads.emplace_back([this] {
                    while(true)
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(m);
                            //sort tasks queue based on time
                            cv.wait(lock, [this] {
                                time_t currentTime = std::time(nullptr);
                                tm* currentTimePtr = std::localtime(&currentTime);
                                return (!tasks.empty()) && std::difftime(std::mktime(currentTimePtr),std::mktime(tasks.top().room.time)) >= 0;
                            });
                            task = std::move(tasks.top().func);
                            tasks.pop();
                        }
                        task();
                    }
                });
            }
        }
        void RoomsInfo(int numberOfRooms){
            std::string Name;
            int capacity;
            for(int i = 0; i < numberOfRooms; i++) 
            {
                std::cout << "Enter the name of the room: " << std::endl;
                std::cin >> Name;
                std::cout << "Enter the capacity of the room: " << std::endl;
                std::cin >> capacity;
                Rooms room = Rooms(Name, capacity);
                roomsFree.insert(std::pair(room, capacity));
            }
            std::map<Rooms, int, comp> roomsFree;
        }

        bool roomCapacityFree(int capacity, Rooms *room, tm* currentTime)
        {
            std::map<Rooms, int>::iterator itr;
            for (itr = roomsFree.begin(); itr != roomsFree.end(); ++itr) 
            {   
                if(itr->second >= capacity && itr->first.available)
                {
                    *room = itr->first;
                    return true;
                }
            }
            return false;
        }

        void RoomsFree(int capacity, int time, std::function<void()> task){
            std::unique_lock<std::mutex> lock(m);
            Rooms roomAllotted;
            struct tm* currentTime;
            time_t tt = std::time(time_t());
	        currentTime = (localtime(&tt));
            cv.wait(lock, [this, &roomAllotted, capacity, currentTime] { return !roomsFree.empty() && roomCapacityFree(capacity, &roomAllotted, currentTime);});

            roomAllotted.time->tm_min = currentTime->tm_min + time;
            roomAllotted.available = false;

            std::cout << "Room Name: " << roomAllotted.name << std::endl;
            std::cout << "Room Time: " << roomAllotted.time->tm_mday << " Day " << roomAllotted.time->tm_mon << " Month " << roomAllotted.time->tm_hour << ":" << roomAllotted.time->tm_min << std::endl;
            std::cout << "Room Capacity: " << roomAllotted.capacity << std::endl;

            tasks.emplace(std::move(task), roomAllotted, roomAllotted.time->tm_min);
            roomsBooked.insert(std::pair(roomAllotted, roomAllotted.capacity));
            roomsFree.erase(roomAllotted);
            cv.notify_one();
        }

};

int main() {
    int numberOfRooms;
    std::cout << "Enter the number of rooms available: " << std::endl;
    std::cin >> numberOfRooms;
    RoomsQueue roomsQueue(numberOfRooms);
    roomsQueue.RoomsInfo(numberOfRooms);
    while(true)
    {
        int capacity;
        int timeNeeded;
        std::cout << "Enter the capacity of the room needed: " << std::endl;
        std::cin >> capacity;
        std::cout << "Enter the time needed for the room: " << std::endl;
        std::cin >> timeNeeded;
        roomsQueue.RoomsFree(capacity, timeNeeded, []{;});
    }
    return 0;
}