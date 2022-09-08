#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <stdexcept>

/**
 * Array of a fixed length.
 */
/**
 * std::size_t is a C++ alternative to the size_t type from C.
 * size_t is typically used when referring to anything that is a quantity
 * related to memory, such as the length of an array (which is in memory).
 * It is an unsigned integer type and is typically (but not always) a typedef
 * for something like unsigned or long unsigned.
 * (If you think this is complicated, then you should look up size_type.)
 */
template <typename T, std::size_t Length>
class Array
{
public:
    /*
     * TODO: Provide a static assertion here that will prohibit arrays
     * of length 0.
     */

    static_assert(Length != 0, "Length of array cannot be zero. \n");

    Array(){}
    ~Array(){}
    Array(const Array& other)
    //copy constructor
    {
        for (std::size_t i = 0; i < Length; i++)
        {
            x[i] = other.at(i);
        }
    };
    
    Array& operator=(const Array& other)
    {
        
        for (std::size_t i = 0; i < Length; i++)
        {
            x[i] = other.at(i);
        }

        return *this;

    };

    /**
     * Returns the size of the array.
     */
    std::size_t size() const
    {
        return Length;
    };


    /**
     * Returns the first or last element of the array.
     *
     * Ideally, there would be non-const overloads for these too.
     */
    const T& front() const
    {
        return x[0];
    };
    const T& back() const
    {
        return x[Length-1];
    };

    /**
     * Returns the element of the array that is at the given index.
     * Throws std::out_of_range if index is out of range.
     */
    T& at(std::size_t index)
    {
        if (index < Length)
        {
            return x[index];
        }

        else
        {
            throw std::out_of_range("Out of Range");
        }
        
    };
    const T& at(std::size_t index) const {
        if (index < Length)
        {
            return x[index];
        }

        else
        {
            throw std::out_of_range("Out of Range");
        }
        
    };

    /**
     * Equality checking, i.e. checking if two arrays contain the exact
     * same elements.
     */
    bool operator==(const Array& other) const
    {
        for (std::size_t i = 0 ; i < Length ; i++)
        {
            if (x.at(i)!= other.at(i))
            t = false;
        }
        return t;
    };
    bool operator!=(const Array& other) const
    {
        for (std::size_t i = 0 ; i < Length ; i++)
        {
            if (x.at(i)!= other.at(i))
            t = false;
        }
        return !t;
    };

private:
    // TODO: Your member variables and helper methods go here.
    T x[Length];
    bool t = true;
};

/**
 * Returns the element of the array that is at the given index.
 * Causes compilation to fail (with a static assertion) if index is out
 * of range.
 */
template <std::size_t Index, typename T, std::size_t Length>
T& get(Array<T, Length>& array)
{
    static_assert(Index<Length, "Index out of range \n");
    return array.at(Index);
     
};
template <std::size_t Index, typename T, std::size_t Length>
const T& get(const Array<T, Length>& array)
{
    static_assert(Index<Length, "Index out of range \n");
    return array.at(Index);
    
};

/**
 * Prints each element of the array.
 */
template <typename T, std::size_t Length>
std::ostream& operator<<(std::ostream& os, const Array<T, Length>& array)
{
    for (std::size_t i = 0; i < array.size() ; i++)
    {
        
        os<<array.at(i)<<" ";
         
    }
    
    std::cout<<"\n";
    return os;
    
    
};

//#include "array.inl"
#endif  // ARRAY_HPP
