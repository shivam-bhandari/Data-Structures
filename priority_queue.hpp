#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "hash_table.hpp"

/**
 * Implementation of a priority queue that supports the
 * extended API discussed during lecture. This priority
 * queue maps unsigned values to instances of ValueType.
 * You are required to use a hash table to efficiently
 * support the operations of the extended API.
 *
 * As with the hash table, any use of the term "element" here
 * refers to a key-value pair.
 *
 * As with the hash table (see the block comment right before the
 * definition of class HahsTable), any mention of "constant time" in
 * double quotes means "amortized constant time on average",
 * i.e. you should use hashing / the required hash table
 * to speed things up and not iterate through the entire underlying
 * binary heap array unnecessarily.
 *
 * The priority queue's underlying implementation is
 * required to be a binary min heap and the hash table
 * that you implement in the first part of this assignment.
 */
template <typename ValueType>
struct Pair
{
    unsigned key;
    ValueType value;
};
bool Pow2(unsigned n)
{
        if(n>0)
        {
            while(n%2 == 0)
            {
                n/=2;
            }
            if(n == 1)
            {
                return true;
            }
        }
        if(n == 0 || n != 1)
        {
            return false;
        }
    return false;
}

template <typename ValueType>
class PriorityQueue
{
private:
    // TODO: Your members here.


    std::unique_ptr<Pair<ValueType>[]> pq;
    HashTable<unsigned> ht{3};
public:
    unsigned size = 0;
    unsigned count = 0;
    unsigned index = 1;


    /**
     * Creates a priority queue that can have at most @maxSize elements.
     *
     * Throws std::runtime_error if @maxSize is 0.
     */
    explicit PriorityQueue(unsigned maxSize)
    {
        if (maxSize == 0) 
        {
            throw std::runtime_error("Table size is zero");
        }

        pq = std::make_unique<Pair<ValueType>[]>(maxSize+1);
        size = maxSize;
        count = 0;
    }

    ~PriorityQueue()
    {

    }

    /**
     * Makes the underlying implementation details (including the max size) look
     * exactly the same as that of @rhs.
     */
    PriorityQueue(const PriorityQueue& rhs)
    {
        pq = std::make_unique<Pair<ValueType>[]>(rhs.size+1);
        for (size_t i = 1; i <= rhs.size; i++)
        {
            pq[i].key = rhs.pq[i].key;
            pq[i].value = rhs.pq[i].value;
        }

        size = rhs.size + 1;
        count = rhs.count;
        index = rhs.index;
        ht = rhs.ht;
    }
    PriorityQueue& operator=(const PriorityQueue& rhs)
    {
        pq = std::make_unique<Pair<ValueType>[]>(rhs.size+1);
        for (size_t i = 1; i <= rhs.size; i++)
        {
            pq[i].key = rhs.pq[i].key;
            pq[i].value = rhs.pq[i].value;
        }

        size = rhs.size + 1;
        count = rhs.count;
        index = rhs.index;
        ht = rhs.ht;
        return *this;
    }

    /**
     * Takes the underlying implementation details of @rhs
     * and gives them to "this" object.
     * After this, @rhs should be in a "moved from" state.
     */
    PriorityQueue(PriorityQueue&& rhs) noexcept
    {
        this->pq = std::move(rhs.pq);
        this->size = rhs.maxSize();
        this->count = rhs.numElements();
        index = rhs.index;
        rhs.count = 0;
        rhs.size = 0;
    }
    PriorityQueue& operator=(PriorityQueue&& rhs) noexcept
    {
        this->pq = std::move(rhs.pq);
        this->size = rhs.maxSize();
        this->count = rhs.numElements();
        index = rhs.index;
        rhs.count = 0;
        rhs.size = 0;
        return *this;
    }

    /*
     * Both of these must run in constant time.
     */
    unsigned numElements() const
    {
        return count;
    }

    unsigned maxSize() const
    {
        return size;
    }

    /**
     * Print the underlying heap level-by-level.
     * See prog_hw4.pdf for how exactly this should look.
     */
    friend std::ostream& operator<<(
        std::ostream& os,
        const PriorityQueue<ValueType>& pq)
    {
        // TODO: Implement this method.
        for (size_t i = 1; i <= pq.numElements(); i++)
        {
            os<<"("<<pq.pq[i].key<<","<<pq.pq[i].value<<") ";
            if(Pow2(i+1) && (i)!=pq.numElements())
            {
                cout<<endl;
            }
            
        }
        os<<endl;
        
        return os;
    }

    /**
     * Inserts a key-value pair mapping @key to @value into
     * the priority queue.
     *
     * Returns true if success.
     * In this case, must run in logarithmic time.
     *
     * Returns false if @key is already in the priority queue
     * or if max size would be exceeded.
     * (In either of these cases, the insertion is not performed.)
     * In this case, must run in "constant time".
     */
    bool insert(unsigned key, const ValueType& value)
    {
        if (count + 1 > this->maxSize())
        {
            return false;
        }
        if (ht.get(key))
        {
            return false;
        }
        count++;
        pq[index].key = key;
        pq[index].value = value;
        index++;
        unsigned pos = index - 1;
        unsigned parent;
        ht.insert(key,pos);
        if (count>1)
        {
            parent = pos/2;
            while(pq[parent].key > pq[pos].key)
            {
                ht.update(pq[parent].key,pos);
                ht.update(pq[pos].key,parent);
                auto tempkey = pq[parent].key;
                pq[parent].key = pq[pos].key;
                pq[pos].key = tempkey;
                auto tempvalue = pq[parent].value;
                pq[parent].value = pq[pos].value;
                pq[pos].value = tempvalue;

                pos = parent;
                parent = pos/2;
            }
        
        }
//        cout<<ht<<endl;
        return true;
    }

    /**
     * Returns key of the smallest element in the priority queue
     * or null pointer if empty.
     *
     * This function must run in constant time.
     *
     * The pointer may be invalidated if the priority queue is modified.
     */
    const unsigned* getMinKey() const
    {
        if (count<1)
        {
            return nullptr;
        }
        
        unsigned* minkey;
        minkey = &(pq[1].key);
        return minkey;
    }

    /**
     * Returns value of the smallest element in the priority queue
     * or null pointer if empty.
     *
     * This function must run in constant time.
     *
     * The pointer may be invalidated if the priority queue is modified.
     */
    const ValueType* getMinValue() const
    {
        if (count<1)
        {
            return nullptr;
        }
        
        ValueType* minvalue;
        minvalue = &(pq[1].value);
        return minvalue;
    }

    /**
     * Removes the root of the priority queue.
     *
     * This function must run in logarithmic time.
     *
     * Returns true if success.
     * Returns false if priority queue is empty, i.e. nothing to delete.
     */
    bool deleteMin()
    {
        if (this->numElements() < 1)
        {
            return false;
        }

        // index is the next index we are going to insert at
        index--;
        count--;
        ht.remove(pq[1].key);
        pq[1] = pq[index];
        unsigned pos = 1;
        unsigned leftchild = 2*pos;
        unsigned rightchild = 2*pos + 1;
        unsigned smallest;

        //cout<<this->numElements()<<endl;
        if (this->numElements() > 1)
        {
            while (1)
            {
                if (leftchild>this->numElements())
                {
                    if (rightchild>this->numElements())
                    {
                        break;
                    }
                    if (pq[pos].key>pq[leftchild].key)
                    {
                        smallest = leftchild;
                        ht.update(pq[smallest].key,pos);
                        ht.update(pq[pos].key,smallest);
                        auto tempkey = pq[smallest].key;
                        pq[smallest].key = pq[pos].key;
                        pq[pos].key = tempkey;
                        auto tempvalue = pq[smallest].value;
                        pq[smallest].value = pq[pos].value;
                        pq[pos].value = tempvalue;                    
                        pos = smallest;
                    }    
                    
                }
                else
                {

                if (pq[pos].key>pq[leftchild].key || pq[pos].key>pq[rightchild].key)
                {
                    if (pq[leftchild].key<pq[rightchild].key)
                    {
                        smallest = leftchild;
                    }
                    else
                    {
                        smallest = rightchild;
                    }
                    ht.update(pq[smallest].key,pos);
                    ht.update(pq[pos].key,smallest);
                    auto tempkey = pq[smallest].key;
                    pq[smallest].key = pq[pos].key;
                    pq[pos].key = tempkey;
                    auto tempvalue = pq[smallest].value;
                    pq[smallest].value = pq[pos].value;
                    pq[pos].value = tempvalue;                    
                    pos = smallest;
                }
                
                }
            leftchild = 2*pos;          
            rightchild = 2*pos + 1;


            }    
        }
    //   cout<<ht<<endl;
        return true;
    }

    /**
     * Returns address of the value that @key is mapped to in the priority queue.
     *
     * These functions must run in "constant time".
     *
     * Returns null pointer if @key is not in the table.
     */
    ValueType* get(unsigned key)
    {
        unsigned* loc = ht.get(key);
        ValueType* val = &pq[*loc].value;
        return val;
    }
    const ValueType* get(unsigned key) const
    {
        unsigned* loc = ht.get(key);
        ValueType* val = &pq[*loc].value;
        return val;
    }

    /**
     * Subtracts/adds @change from/to the key of
     * the element that has key @key.
     *
     * These functions must run in "constant time" + logarithmic time.
     * This means you must use the required hash table to find the
     * location of @key in the underlying binary heap array.
     *
     * Returns true if success.
     * Returns false if any of the following:
     * - @change is 0.
     * - @key not found.
     * - If the change would lead to a duplicate key.
     *
     * The function does not do anything about  overflow/underflow.
     * For example, an operation like decreaseKey(2, 10)
     * has an undefined effect.
     */
    bool decreaseKey(unsigned key, unsigned change)
    {
//        cout<<ht<<endl;
        if (change == 0)
        {
            return false;
        }
        if (ht.get(key) == nullptr)
        {
            return false;
        }
        unsigned* loc = ht.get(key);

        if (ht.get(pq[*loc].key - change))
        {
            return false;
        }
        pq[*loc].key -= change;
//        cout<<pq[*loc].key<<endl;
        unsigned pos = *loc;
        unsigned parent;
        ht.insert(pq[*loc].key,pos);
        ht.remove(key);
//        cout<<ht<<endl;
        if (count>1)
        {
            parent = pos/2;
            while(pq[parent].key > pq[pos].key)
            {
                ht.update(pq[parent].key,pos);
                ht.update(pq[pos].key,parent);
                auto tempkey = pq[parent].key;
                pq[parent].key = pq[pos].key;
                pq[pos].key = tempkey;
                auto tempvalue = pq[parent].value;
                pq[parent].value = pq[pos].value;
                pq[pos].value = tempvalue;

                pos = parent;
                parent = pos/2;
            }
        
        }

//        cout<<ht<<endl;

        return true;

    }
    bool increaseKey(unsigned key, unsigned change)
    {
        if (change == 0)
        {
            return false;
        }
        if (ht.get(key) == nullptr)
        {
            return false;
        }
        unsigned* loc = ht.get(key);
        if (ht.get(pq[*loc].key + change))
        {
            return false;
        }
        pq[*loc].key += change;
        unsigned pos = *loc;
        unsigned leftchild = 2*pos;
        unsigned rightchild = 2*pos + 1;
        unsigned smallest;

        ht.insert(pq[*loc].key,pos);
        ht.remove(key);

        //cout<<this->numElements()<<endl;
        if (this->numElements() > 1)
        {
            while (1)
            {
                if (leftchild>this->numElements())
                {
                    if (rightchild>this->numElements())
                    {
                        break;
                    }
                    if (pq[pos].key>pq[leftchild].key)
                    {

                        smallest = leftchild;
                        ht.update(pq[smallest].key,pos);
                        ht.update(pq[pos].key,smallest);
                        auto tempkey = pq[smallest].key;
                        pq[smallest].key = pq[pos].key;
                        pq[pos].key = tempkey;
                        auto tempvalue = pq[smallest].value;
                        pq[smallest].value = pq[pos].value;
                        pq[pos].value = tempvalue;                    
                        pos = smallest;
                        leftchild = 2*pos;
                        rightchild = 2*pos + 1;
                    }    
                    
                }
                else
                {

                if (pq[pos].key>pq[leftchild].key || pq[pos].key>pq[rightchild].key)
                {
                    if (pq[leftchild].key<pq[rightchild].key)
                    {
                        smallest = leftchild;
                    }
                    else
                    {
                        smallest = rightchild;
                    }
                    ht.update(pq[smallest].key,pos);
                    ht.update(pq[pos].key,smallest);
                    auto tempkey = pq[smallest].key;
                    pq[smallest].key = pq[pos].key;
                    pq[pos].key = tempkey;
                    auto tempvalue = pq[smallest].value;
                    pq[smallest].value = pq[pos].value;
                    pq[pos].value = tempvalue;                    
                    pos = smallest;
                    leftchild = 2*pos;
                    rightchild = 2*pos + 1;
                }
                
                }

            }    
        }
    // cout<<ht<<endl;
    return true;
    }

    /**
     * Removes element that has key @key.
     *
     * These functions must run in "constant time" + logarithmic time.
     * This means you must use the required hash table to find the
     * location of @key in the underlying binary heap array.
     *
     * Returns true if success.
     * Returns false if @key not found.
     */
    bool remove(unsigned key)
    {

        //can go either way, percolate up or down
        unsigned flag = 0;
        unsigned* loc = ht.get(key);
        if (ht.get(key) == nullptr )
        {
            return false;
        }
        index--;
        ht.remove(pq[*loc].key);
        pq[*loc] = pq[index];

        if (pq[*loc/2].key > pq[*loc].key)
        {
            unsigned pos = *loc;
            unsigned parent;
            ht.insert(pq[*loc].key,pos);
            ht.remove(key);
            if (count>1)
            {
                parent = pos/2;
                while(pq[parent].key > pq[pos].key)
                {
                    ht.update(pq[parent].key,pos);
                    ht.update(pq[pos].key,parent);
                    auto tempkey = pq[parent].key;
                    pq[parent].key = pq[pos].key;
                    pq[pos].key = tempkey;
                    auto tempvalue = pq[parent].value;
                    pq[parent].value = pq[pos].value;
                    pq[pos].value = tempvalue;

                    pos = parent;
                    parent = pos/2;
                }
            
            }

        }
        else
        {

            unsigned pos = *loc;
            unsigned leftchild = 2*pos;
            unsigned rightchild = 2*pos + 1;
            unsigned smallest;

            //cout<<this->numElements()<<endl;
            if (this->numElements() > 1)
            {
                while (1)
                {
                    if (leftchild>this->numElements())
                    {
                        if (rightchild>this->numElements())
                        {
                            break;
                        }
                        if (pq[pos].key>pq[leftchild].key)
                        {
                            flag = 1;
                            smallest = leftchild;
                            ht.update(pq[smallest].key,pos);
                            ht.update(pq[pos].key,smallest);
                            auto tempkey = pq[smallest].key;
                            pq[smallest].key = pq[pos].key;
                            pq[pos].key = tempkey;
                            auto tempvalue = pq[smallest].value;
                            pq[smallest].value = pq[pos].value;
                            pq[pos].value = tempvalue;                    
                        }    
                    
                    }
                    else
                    {

                        if (pq[pos].key>pq[leftchild].key || pq[pos].key>pq[rightchild].key)
                        {
                            if (pq[leftchild].key<pq[rightchild].key)
                            {
                                smallest = leftchild;
                            }
                            else
                            {
                                smallest = rightchild;
                            }
                            ht.update(pq[smallest].key,pos);
                            ht.update(pq[pos].key,smallest);
                            auto tempkey = pq[smallest].key;
                            pq[smallest].key = pq[pos].key;
                            pq[pos].key = tempkey;
                            auto tempvalue = pq[smallest].value;
                            pq[smallest].value = pq[pos].value;
                            pq[pos].value = tempvalue;                    

                        }
                
                    }
                    pos = smallest;
                    leftchild = 2*pos;
                    if (flag != 1)
                    {                    
                        rightchild = 2*pos + 1;
                    }


                }    
            }

        }
         
        count--;
        return true;    
    }

};
//#include "priority_queue.inl"
#endif  // PRIORITY_QUEUE_HPP
