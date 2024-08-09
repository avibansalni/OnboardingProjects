#include <iostream>
#include <cstring>

class Pointer {
    public:
        size_t size = 0;
        Pointer* next;
        bool isFree = true;
        Pointer(size_t size) : size(size), next(nullptr) {};
};

class Allocator {
    private:
        size_t poolSize;
        Pointer* pointerList;

    public:
        Allocator(size_t size): poolSize(size), pointerList(nullptr) {
            pointerList = new Pointer(size);
        }

        Pointer* createBlock(size_t blocksize, Pointer* current) {
            Pointer* newBlock = current;
            Pointer* temp = newBlock;
            int headSize = temp->size-blocksize;
            int size = blocksize;

            while(size>0) {
                if(temp->next==nullptr) {
                    temp->next = new Pointer(poolSize);
                }
                temp->size = headSize;
                temp->isFree = false;
                temp->next = current->next;
                current = current->next;
                temp = temp->next;
                size--;
            }
            return newBlock;
        }

        void* allocate(size_t size) {
            if (poolSize < 0) {
                std::cout << "Memory allocation failed" << std::endl;
                return nullptr;
            }
            std::cout << "Memory allocated successfully" << std::endl;
            Pointer* current = pointerList;
            
            while (true) {
                if(current->size >= size && current->isFree) {
                    poolSize = poolSize - size;
                    return createBlock(size, current);;
                } 
                else
                {
                    if(current->next == nullptr) {
                        current->next = new Pointer(poolSize);
                    }
                    current = current->next;
                }
                
            }
            return nullptr;
        }

        void deallocate(void* ptr) {
            Pointer* current = pointerList;
            Pointer* prev = nullptr;
            int pointerFree = 0;
            while(current) {
                if(current == ptr) {
                    int size = current->size;
                    while(size == current->size) {
                        pointerFree++;
                        current->isFree = true;
                        current = current->next;
                    }
                    poolSize = poolSize + pointerFree;
                    return;
                }
                prev = current;
                current = current->next;
            }
        }

};

class Myobj {
    public:
        void* allocation(Allocator& allocator, int size) 
        {
            return allocator.allocate(size);
        }

        void deleteAllocation(Allocator& allocator, void* ptr)
        {
            allocator.deallocate(ptr);
        }
};

int main() {

    Allocator allocate(30);
    Myobj obj1;
    void* a = obj1.allocation(allocate, 10);
    void* b = obj1.allocation(allocate, 20);
    obj1.deleteAllocation(allocate, a);
    return 0;
}
