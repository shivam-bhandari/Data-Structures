#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

using namespace std;
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <memory>
/**
 * Implementation of a hash table that stores key-value
 * pairs mapping unsigned integers to instances of
 * ValueType.
 *
 * Hash function: key % tableSize
 * Collision resolution: quadratic probing.
 * Non-unique keys are not supported.
 *
 * Any use of the term "element" refers to a key-value pair.
 *
 * Some functions have runtime requirements.
 * During lecture, I mentioned that when talking about hash
 * tables, people tend to talk as if common hash table
 * operations take constant time, even though this is false.
 * Here, any mention of "constant time" in double quotes means
 * "amortized constant time on average", i.e. you should
 * use hashing to speed things up and not iterate through
 * all the buckets, ignoring the hash function. For instance,
 * it is required that you implement delete() in "constant
 * time", so you cannot take the inefficient approach of
 * searching for the relevant key by iterating 
 * bucket-by-bucket.
 *
 * The table rehashes whenever the insertion of a new
 * element would put the load factor at at least 1/2.
 * (The rehashing is done before the element would've been inserted.)
 * Upon a rehash, the table size (let's call it m) should
 * be increased to the lowest prime number that is greater
 * than or equal to 2m. Elements are then transferred
 * from the "old table" to the "new/larger table" in the
 * order in which they appear in the old table, and then
 * the new element is finally inserted.
 */
template <typename ValueType >
struct Node
{
    unsigned key;
    ValueType value;
    bool isFilled = false;
    bool isEmpty = true;
//    bool isDeleted;
};
template <typename ValueType>
class HashTable
{

private:

    // TODO: Your members here.
    bool isPrime(int num)
    {
        bool flag = true;
        for(int i = 2; i <= num / 2; i++)
        {
        if(num % i == 0)
            {
            flag = false;
            break;
            }
        }
        return flag;
    }


    unsigned modfunction(unsigned key,unsigned size)
    {
        return key%size;
    }

    bool checkForRehashing(unsigned count,unsigned size)
    {
        float loadfactor = (float)count/size;
        if (loadfactor < 0.5)
        return false;
        else
        return true;
        
    }

    unsigned nextPrime(unsigned N)
    {
        if (N <= 1)
            return 2;
    
        unsigned prime = N;
        bool found = false;
        while (!found) 
        {
            prime++;
            if (isPrime(prime))
                found = true;
        }
        return prime;
    }

    // unsigned nextPerfectSquare(unsigned N)
    // {
    //     unsigned nextN = floor(sqrt(N)) + 1;
    
    //     return nextN * nextN;
    // }

    std::unique_ptr<Node<ValueType>[]> table;


public:
    unsigned size = 0;
    unsigned count = 0;
    /**
     * Creates a hash table with the given number of
     * buckets/slots.
     *
     * Throws std::runtime_error if @tableSize is 0 or not
     * prime.
     */
    explicit HashTable(unsigned tableSize)
    {
        if (tableSize == 0 || !isPrime(tableSize)) 
        {
            throw std::runtime_error("Table size is zero or not a prime");
        }

        table = std::make_unique<Node<ValueType>[]>(tableSize);
        size = tableSize;
        count = 0;
        // for (size_t i = 0; i < tableSize; i++)
        // {
        //     table[i].isEmpty = true;
        //     table[i].isFilled = false;
        // }
        
        
    }

    ~HashTable(){}

    /**
     * Makes the underlying hash table of this object look
     * exactly the same as that of @rhs.
     */
    HashTable(const HashTable& rhs)
    {
        unsigned count = 0;
        table = std::make_unique<Node<ValueType>[]>(rhs.tableSize());
        for (size_t i = 0; i < rhs.tableSize(); i++)
        {
            if (!rhs.table[i].isFilled)
            {
                table[i].isFilled = rhs.table[i].isFilled;
                table[i].isEmpty = rhs.table[i].isEmpty ;
            }
            else
            {
                table[i].key = rhs.table[i].key;
                table[i].value = rhs.table[i].value;
                table[i].isFilled = rhs.table[i].isFilled;
                table[i].isEmpty = rhs.table[i].isEmpty ;   
                count++;           
            }
        }
        this->size = rhs.tableSize();
        this->count = count;
        
    }
    HashTable& operator=(const HashTable& rhs)
    {
        table = std::make_unique<Node<ValueType>[]>(rhs.tableSize());
        for (size_t i = 0; i < rhs.tableSize(); i++)
        {
            if (!rhs.table[i].isFilled)
            {
                table[i].isFilled = rhs.table[i].isFilled;
                table[i].isEmpty = rhs.table[i].isEmpty ;
            }
            else
            {
                table[i].key = rhs.table[i].key;
                table[i].value = rhs.table[i].value;
                table[i].isFilled = rhs.table[i].isFilled;
                table[i].isEmpty = rhs.table[i].isEmpty ;          
            }            
            
        }

        this->size = rhs.tableSize();
        this->count = rhs.numElements();
        return *this;

    }

    /**
     * Takes the underlying implementation details of @rhs
     * and gives them to "this" object.
     * After this, @rhs should be in a "moved from" state.
     */
    HashTable(HashTable&& rhs) noexcept
    {
        //*this = std::move(rhs);
        this->table = std::move(rhs.table);
        this->size = rhs.tableSize();
        this->count = rhs.numElements();
        rhs.count = 0;
        rhs.size = 0;
    }
    HashTable& operator=(HashTable&& rhs) noexcept
    {
        //*this = std::move(rhs);
        this->table = std::move(rhs.table);
        this->size = rhs.tableSize();
        this->count = rhs.numElements();
        rhs.count = 0;
        rhs.size = 0;
        return *this;
    }

    /**
     * Both of these must run in constant time.
     */
    unsigned tableSize() const
    {
        return size;
    }

    unsigned numElements() const
    {
        return count;
    }

    /**
     * Prints each bucket in the hash table.
     * See prog_hw4.pdf for how exactly this should look.
     *
     * I don't think that a friend of a templated class can
     * be defined outside of the class. If you can figure
     * out how to do that in a way that works on the CSIF,
     * go for it.
     */
    friend std::ostream& operator<<(std::ostream& os,
                                    const HashTable<ValueType>& ht)
    {
        // TODO: Implement this method.
        for (size_t i = 0; i < ht.tableSize(); i++)
        {
            if (!ht.table[i].isFilled)
            {
                os<<"Bucket "<<i<<": (empty)"<<endl;
            }
            else
            {
                os<<"Bucket "<<i<<": "<<ht.table[i].key<<" -> "<<ht.table[i].value<<endl;
            }
            
        }    
        return os;
    }

    /**
     * Inserts a key-value pair mapping @key to @value into
     * the table.
     *
     * This function must run in "constant time".
     *
     * Returns true if success.
     * Returns false if @key is already in the table
     * (in which case, the insertion is not performed).
     */
    bool insert(unsigned key, const ValueType& value)
    {
        std::unique_ptr<Node<ValueType>[]> newTable;
        
        if (checkForRehashing(count+1,size))
        {
            //rehash
            newTable = std::make_unique<Node<ValueType>[]>(nextPrime(2 * this->tableSize()));
            unsigned newSize = nextPrime(2 * this->tableSize());
            for (size_t i = 0; i < this->tableSize(); i++)
            {
                if (!table[i].isEmpty)
                {
                    //newTable.insert(table[i].key,table[i].value);
                    unsigned ind = table[i].key%newSize;
                    unsigned p = ind;
                    unsigned j = 0;

                    while(newTable[p].isFilled)
                    {

                    ++j;
                    p += (2 * j) - 1;
                    if(p >= this->tableSize())
                    p -= this->tableSize();

                    }

                    if (table[i].key == key)
                    {
                        return false;
                    }
                    

                    newTable[p].key = table[i].key;
                    newTable[p].value = table[i].value;
                    newTable[p].isEmpty = false;
                    newTable[p].isFilled = true;                    
                }                
            }
            table = std::move(newTable);
            size = nextPrime(2 * this->tableSize());
        }
        

        unsigned index = modfunction(key,this->tableSize());
        unsigned pos = index;
        unsigned i = 0;

        while(table[pos].isFilled)
        {
            if (table[pos].key == key)
            {
                return false;
            }
        ++i;
        pos = pos + (2 * i) - 1;
        if(pos >= this->tableSize())
        pos -= this->tableSize();

        }         
        table[pos].key = key;
        table[pos].value = value;
        table[pos].isEmpty = false;
        table[pos].isFilled = true;

        count++;


        return true;

    }

    /**
     * Finds the value corresponding to the given key
     * and returns its address.
     *
     * This function must run in "constant time".
     *
     * Returns null pointer if @key is not in the table.
     */
    ValueType* get(unsigned key)
    {
        //cout<<"Seg fault in the beginning of the function itself."<<endl;
        unsigned index = modfunction(key,this->tableSize());
        unsigned pos = index;
        unsigned i = 0;
        bool found = false;


        //accomodate for isDeleted

        // cout<<"Seg fault here."<<endl;
        if (table[pos].key == key && table[pos].isFilled)
            {
                found = true;
            }
        
        while(!(table[pos].isEmpty))
        {
            if (table[pos].key == key && table[pos].isFilled)
            {
                found = true;
                break;
            }
        ++i;
        pos += (2 * i) - 1;
        if(pos >= this->tableSize())
        pos -= this->tableSize();
        }

        if (found == false)
        {
            return nullptr;
        }
        ValueType* val;
        val = &(table[pos].value);
        return val;

    }
    const ValueType* get(unsigned key) const
    {
        unsigned index = key%size;
        unsigned pos = index;
        unsigned i = 0;
        bool found = false;
        //accomodate for isDeleted
        if (table[pos].key == key && table[pos].isFilled)
            {
                found = true;
            }
        while(!(table[pos].isEmpty))
        {
            if (table[pos].key == key && table[pos].isFilled)
            {
                found = true;
                break;
            }
            
        ++i;
        pos += (2 * i) - 1;
        if(pos >= this->tableSize())
        pos -= this->tableSize();
        }

        if (found == false)
        {
            return nullptr;
        }
        ValueType* val;
        val = &(table[pos].value);
        return val;
    }

    /**
     * Updates the key-value pair with key @key to be
     * mapped to @newValue.
     *
     * This function must run in "constant time".
     *
     * Returns true if success.
     * Returns false if @key is not in the table.
     */
    bool update(unsigned key, const ValueType& newValue)
    {
        unsigned index = modfunction(key,this->tableSize());
        unsigned pos = index;
        unsigned i = 0;
        bool found = false;
        //accomodate for isDeleted
        while(!table[pos].isEmpty)
        {
            if (table[pos].key == key && table[pos].isFilled)
            {
                found = true;
                break;
            }
            
        ++i;
        pos += (2 * i) - 1;
        if(pos >= this->tableSize())
        pos -= this->tableSize();
        }

        if (found == false)
        {
            return false;
        }

        table[pos].value = newValue;
        return true;
    }

    /**
     * Deletes the element that has the given key.
     *
     * This function must run in "constant time".
     *
     * Returns true if success.
     * Returns false if @key not found.
     */
    bool remove(unsigned key)
    {
        unsigned index = modfunction(key,this->tableSize());
        unsigned pos = index;
        unsigned i = 0;
        bool found = false;
        //accomodate for isDeleted
        while(!(table[pos].isEmpty))
        {
            if (table[pos].key == key && table[pos].isFilled)
            {
                found = true;
                break;
            }
            
        ++i;
        pos += (2 * i) - 1;
        if(pos >= this->tableSize())
        pos -= this->tableSize();
        }

        if (found == false)
        {
            return false;
        }

        count--;
        table[pos].isFilled = false;
        return true;

    }

    /**
     * Deletes all elements that have the given value.
     *
     * Returns the number of elements deleted.
     */
    unsigned removeAllByValue(const ValueType& value)
    {
        unsigned num = 0;
        for (size_t i = 0; i < this->tableSize(); i++)
        {
            if (table[i].value == value && table[i].isFilled)
            {
                table[i].isFilled = false;
                count--;
                num++;
            }
            
        }
        return num; 
    }

    /**
     * Two instances of HashTable<ValueType> are considered 
     * equal if they contain the same elements, even if those
     * elements are in different buckets (i.e. even if the
     * hash tables have different sizes).
     */
    bool operator==(const HashTable& rhs) const
    {
        unsigned num = 0;
        if (this->numElements() != rhs.numElements())
        {
            return false;
        }
        
        for (size_t i = 0; i < this->tableSize(); i++)
        {
            if (this->table[i].isFilled)
            {
                for (size_t j = 0; j < rhs.tableSize(); j++)
                {
                    if (rhs.table[j].isFilled)
                    {                
                        if ((this->table[i].key == rhs.table[j].key) && (this->table[i].value == rhs.table[j].value))
                        {
                            num++;
                        }                
                    }
                    
                }
            }
                    
        }

        if (num == this->numElements())
        {
            return true;
        }
        
        return false;
        
    }
    bool operator!=(const HashTable& rhs) const
    {
        unsigned num = 0;
        for (size_t i = 0; i < this->tableSize(); i++)
        {
            for (size_t j = 0; j < rhs.tableSize(); j++)
            {
                if (this->table[i].isFilled && rhs.table[j].isFilled)
                {                
                    if ((this->table[i].key == rhs.table[j].key) && (this->table[i].value == rhs.table[j].value))
                    {
                        num++;
                    }                
                }
                
            }
                    
        }

        if (num == this->count)
        {
            return false;
        }
        
        return true;
    }

    /**
     * Returns a newly constructed hash table that is the hash table
     * that would result from inserting each element from @rhs
     * (in the order that they appear in the buckets)
     * into this (i.e. *this) hash table.
     */
    HashTable operator+(const HashTable& rhs) const
    {
        //HashTable<ValueType> ht(this->tableSize());
        HashTable<ValueType> ht(*this);
        for (size_t i = 0; i < rhs.tableSize(); i++)
        {
            if (rhs.table[i].isFilled)
            {                           
                ht.insert(rhs.table[i].key,rhs.table[i].value);
            }

        }

        return ht;
    }



};  

//#include "hash_table.inl"
#endif  // HASH_TABLE_HPP