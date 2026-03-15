template <typename T>
class SharedPtr{
    private:
        T* ptr;
        int* ref_count;

    public:
        explicit SharedPtr(T* p = nullptr){
            if(p == nullptr){
                ptr = nullptr;
                ref_count = nullptr;
            }else{
                ptr = p;
                ref_count = new int(1);
            }
        }

        ~SharedPtr(){
            if(ref_count != nullptr){
                (*ref_count)--;
                if((*ref_count) == 0){
                    delete ptr;
                    delete ref_count;
                }
            }
        }

        T& operator*() const { return *ptr;}
        T* operator->() const {return ptr;}
        T* get() const {return ptr;}

        SharedPtr(const SharedPtr& obj){
            ptr = obj.ptr;
            ref_count = obj.ref_count;

            if(ref_count != nullptr) { 
                (*ref_count)++;
            }
        }

        SharedPtr(SharedPtr&& obj) noexcept {
            ptr = obj.ptr;
            ref_count = obj.ref_count;

            obj.ptr = nullptr;
            obj.ref_count = nullptr;
        }

        SharedPtr& operator=(const SharedPtr& obj){
            if(this == &obj) return *this;

            if(ref_count != nullptr){
                (*ref_count)--;
                if((*ref_count) == 0){
                    delete ptr;
                    delete ref_count;
                }
            }

            ptr = obj.ptr;
            ref_count = obj.ref_count;

            if(ref_count != nullptr){
                (*ref_count)++;
            }

            return *this;
        }

        SharedPtr& operator=(SharedPtr&& obj) noexcept {
            if(this == &obj) return *this;
            
            if(ref_count != nullptr){
                (*ref_count)--;
                if((*ref_count) == 0){
                    delete ptr;
                    delete ref_count;
                }
            }

            ptr = obj.ptr;
            ref_count = obj.ref_count;

            obj.ptr = nullptr;
            obj.ref_count = nullptr;

            return *this;
        }
};

int main(){
    return 0;
}