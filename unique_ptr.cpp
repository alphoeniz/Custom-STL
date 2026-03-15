template <typename T>
class MyUniquePtr{
    private:
        T* ptr;

    public:
        explicit MyUniquePtr(T* p = nullptr) : ptr(p) {}
        ~MyUniquePtr(){
            delete(ptr);
        }
        MyUniquePtr(const MyUniquePtr& obj) = delete;
        MyUniquePtr& operator=(const MyUniquePtr& obj) = delete;
        MyUniquePtr(MyUniquePtr &&obj) noexcept {
            ptr = obj.ptr;
            obj.ptr = nullptr;
        }
        MyUniquePtr& operator=(MyUniquePtr&& obj) noexcept {
            if(this == &obj) return *this;

            delete ptr;

            ptr = obj.ptr;
            obj.ptr = nullptr;

            return *this;
        }
        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }
        T* get() const { return ptr; }
};

int main(){
    return 0;
}