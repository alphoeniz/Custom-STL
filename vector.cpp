#include <iostream>

template<typename T>
class MyVector{

    private:
        size_t size = 0;
        size_t cap = 0;
        T* ptr = nullptr;

    public:
        MyVector(){
            ptr = nullptr;
            size = 0;
            cap = 0;
        }

        ~MyVector(){
            for(size_t i = 0; i < size; i++){
                ptr[i].~T();
            }
            ::operator delete(ptr);
            
            ptr = nullptr;
            size = 0;
            cap = 0;
        }

        MyVector(const MyVector& obj){
            size = obj.size;
            cap = obj.cap;
            ptr = static_cast<T*>(::operator new(cap * sizeof(T)));
            for(size_t i = 0; i < size; i++){
                new (ptr + i) T(obj.ptr[i]);
            }
        }

        MyVector(MyVector&& obj) noexcept {
            size = obj.size;
            cap = obj.cap;
            ptr = obj.ptr;

            obj.size = 0;
            obj.cap = 0;
            obj.ptr = nullptr;
        }

        MyVector& operator=(const MyVector& obj) {
            if(this == &obj) return *this;

            for(size_t i = 0; i < size; i++){
                ptr[i].~T();
            }
            ::operator delete(ptr);

            size = obj.size;
            cap = obj.cap;
            ptr = static_cast<T*>(::operator new(cap * sizeof(T)));
            for(size_t i = 0; i < size; i++){
                new (ptr + i) T(obj.ptr[i]);
            }

            return *this;
        }

        MyVector& operator=(MyVector&& obj) noexcept {
            if(this == &obj) return *this;

            for(size_t i = 0; i < size; i++){
                ptr[i].~T();
            }
            ::operator delete(ptr);

            size = obj.size;
            cap = obj.cap;
            ptr = obj.ptr;

            obj.size = 0;
            obj.cap = 0;
            obj.ptr = nullptr;

            return *this;
        }

        void push_back(const T& val){
            if(size == cap){
                size_t new_cap = (cap == 0) ? 1 : (cap * 2);
                reserve(new_cap);
            }
            new (ptr + size) T(val);
            size++;
        }

        void push_back(T&& val){
            if(size == cap){
                size_t new_cap = (cap == 0) ? 1 : (cap * 2);
                reserve(new_cap);
            }
            new (ptr + size) T(std::move(val));
            size++;
        }

        void reserve(size_t new_cap){
            if(new_cap <= cap) return;
            T* new_ptr = static_cast<T*>(::operator new(new_cap * sizeof(T)));
            for(size_t i = 0; i < size; i++){
                new (new_ptr + i) T(std::move_if_noexcept(ptr[i]));
            }
            for(size_t i = 0; i < size; i++){
                ptr[i].~T();
            }
            ::operator delete(ptr);
            ptr = new_ptr;
            cap = new_cap;
        }

        void pop_back(){
            if(size == 0) return;
            ptr[size-1].~T();
            size--;
        }

        size_t length() const {return size;}
        size_t capacity() const {return cap;}
        T* begin() {return ptr;}
        T* end() {return ptr + size;}
        const T* begin() const {return ptr;}
        const T* end() const {return ptr + size;}

        void print(){
            for(size_t i = 0; i < size; i++){
                std::cout << ptr[i] << " ";
            } std::cout << std::endl;
        }
};

// The Instrumented Class
struct Tracker {
    int id;

    // Default & Parameterized Constructor
    Tracker(int i = 0) : id(i) { 
        std::cout << "  [+] Constructed: " << id << "\n"; 
    }
    
    // Copy Constructor
    Tracker(const Tracker& other) : id(other.id) { 
        std::cout << "  [C] Copy Constructed: " << id << "\n"; 
    }
    
    // Move Constructor
    Tracker(Tracker&& other) noexcept : id(other.id) { 
        std::cout << "  [M] Move Constructed: " << id << "\n"; 
    }
    
    // Destructor
    ~Tracker() { 
        std::cout << "  [-] Destroyed: " << id << "\n"; 
    }

    // Overload for your print() function
    friend std::ostream& operator<<(std::ostream& os, const Tracker& t) {
        return os << t.id;
    }
};

int main() {
    std::cout << "=== TEST 1: Push Back & Reallocation ===\n";
    MyVector<Tracker> v1;
    // Pushing an rvalue (temporary) will trigger your Move push_back
    v1.push_back(Tracker(1)); 
    // This second push should trigger reserve(), moving '1' to new memory
    v1.push_back(Tracker(2)); 

    std::cout << "\n=== TEST 2: Copy Constructor ===\n";
    MyVector<Tracker> v2 = v1; // Should print [C] twice
    std::cout << "v2 size: " << v2.length() << "\n";

    std::cout << "\n=== TEST 3: Move Constructor ===\n";
    MyVector<Tracker> v3 = std::move(v1); // Should print NOTHING (O(1) pointer steal)
    std::cout << "v3 size: " << v3.length() << ", v1 size: " << v1.length() << " (v1 should be empty)\n";

    std::cout << "\n=== TEST 4: Copy Assignment ===\n";
    MyVector<Tracker> v4;
    v4.push_back(Tracker(99));
    std::cout << "  -- Assigning v2 to v4 --\n";
    v4 = v2; // Should destroy 99, then copy 1 and 2

    std::cout << "\n=== TEST 5: Move Assignment ===\n";
    MyVector<Tracker> v5;
    v5 = std::move(v3); // Should steal pointers. v3 becomes empty.

    std::cout << "\n=== TEST 6: Self Assignment ===\n";
    v2 = v2; // Should do absolutely nothing due to your `this == &obj` check
    
    std::cout << "\n=== TEST 7: Pop Back ===\n";
    v5.pop_back(); // Should destroy '2'
    
    std::cout << "\n=== END OF SCOPE (Destructors will fire) ===\n";
    return 0;
}