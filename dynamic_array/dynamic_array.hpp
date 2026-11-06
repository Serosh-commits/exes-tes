#pragma once

#include "cstddef"
#include "memory"
#include "utility"

template<typename T>
class dynamic_array{
  private:
    
    using allocator_type = std::allocator<T>;
    using traits = std::allocator_traits<allocator_type>;
    
    allocator_type alloc_;
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
  public:
    
    dynamic_array()
      :data_(nullptr),size_(0),capacity_(0) { };
    
    std::size_t size() const {
      return size_;
    }
    
    std::size_t capacity() const {
      return capacity_;
    }

    bool isempty() const{
      return size_ == 0;
    }

    void reserve(std::size_t new_capacity){
      if(new_capacity <= capacity_){
        return;
      }

      T* new_data = traits::allocate(alloc_, new_capacity);

      std::size_t constructed = 0;
      try{
        for (; constructed < size_; ++constructed) {
          
          traits::construct(
                alloc_,
                new_data + constructed,
                std::move_if_noexcept(data[constructed])
              );
        }
      } catch (){

        std::destroy_n(new_data, constructed);
        traits::deallocate(alloc_, new_data, new_capacity);
        throw;
      }

      std::destroy_n(data_, size_);

      if(data_ != nullptr){
        traits::deallocate(alloc_, data_, capacity_);
      }

      data_ = new_data;
      capacity = new_capacity;
    }
    ~dynamic_array(){
      std::destroy_n(data_, size_);

      if(data != nullptr){
        traits::deallocate(alloc_, data_, capacity_);
      }
      delete[] data_;
    }

};
