#include "cursor_list.hpp"

#include <iostream>

#define NUM_SLOTS 30

struct Slot
{
    int data;
    unsigned next;  // index of next slot
};

static struct Slot SLOTS[NUM_SLOTS];

void initialize_free()
{
    for (int i = 0; i < NUM_SLOTS - 1; i++)
    {
        SLOTS[i].next = i + 1;
    }
    SLOTS[NUM_SLOTS - 1].next = 0;
        
}

bool firstcreate()
{
    for (int i = 0; i < NUM_SLOTS ; i++)
    {
        if (SLOTS[i].next != 0)
        {
            return 0;
        }
        
    }

    return 1;   
}

unsigned slotAlloc()
{
    // TODO: Implement.

    if (firstcreate())
    {
        initialize_free();
    }
    

    unsigned t; 
    t = SLOTS[0].next;
    SLOTS[0].next = SLOTS[t].next;

    if (t == 0)
    {
        throw std::runtime_error("Wrong input");
    }
    else
        return t;
    
}

void slotFree(unsigned index)
{
    // TODO: Implement.
    SLOTS[index].next = SLOTS[0].next;
    SLOTS[0].next = index;
}

void printSlots(unsigned start, unsigned end)
{
    for (unsigned i = start; i < end; ++i)
        std::cout << "SLOTS[" << i << "]: " << SLOTS[i].data
            << ' ' << SLOTS[i].next << '\n';
}

unsigned clCreate()
{
    // TODO: Implement.
    unsigned t = slotAlloc();
    SLOTS[t].next = 0;
    return t;
}

void clDestroy(unsigned head)
{
    // TODO: Implement.
    // unsigned t = SLOTS[head].next;
    // while (t != 0)
    // {
    //     slotFree(t);
    //     t = SLOTS[t].next;
    // }    
    // slotFree(t);
   
    // unsigned p = head;
    // unsigned t = SLOTS[head].next;
    // while( (t != 0) )
    // {
    //     SLOTS[p].next = SLOTS[t].next;
    //     slotFree(p);
    //     p = t;
    //     t = SLOTS[t].next;
    // }


    // losing header node -- not working !! find solution
    //incorrect order

    unsigned p = head;
    unsigned t = SLOTS[head].next;
    while (SLOTS[t].next != 0)
    {
        slotFree(p);
        p = t;
        t = SLOTS[t].next;
    }
    slotFree(p);
    slotFree(t);

}

void clAppend(unsigned node, int val)
{
    // TODO: Implement.
    unsigned t,p;
    p = slotAlloc();
    t = node;
    // go to end of node and add val

    while (SLOTS[t].next != 0)
    {
        t = SLOTS[t].next;
    }
    
    SLOTS[p].data = val;
    SLOTS[p].next = 0;
    SLOTS[t].next = p;
    
}

void clInsert(unsigned node, int val)
{
    // TODO: Implement.


    unsigned t;
    t = slotAlloc();
    SLOTS[t].data = val;
    SLOTS[t].next = SLOTS[node].next;
    SLOTS[node].next = t;
    
}

unsigned clFind(unsigned head, int val)
{
    // TODO: Implement.
    unsigned t;
    t = SLOTS[head].next;
    while( (t != 0) && SLOTS[t].data != val )
    {
        t = SLOTS[t].next;
    }
    return t;
}

void clPrint(unsigned head)
{
    // TODO: Implement.
    unsigned t;
    t = SLOTS[head].next;
    std::cout<<"SLOTS["<<head<<"]: (header)"<<std::endl;
    while(t != 0)
    {   
        std::cout<<"SLOTS["<<t<<"]: "<<SLOTS[t].data<<std::endl;
        t = SLOTS[t].next;
    }
    
}

bool clDelete(unsigned head, int val)
{
    // TODO: Implement.
    // unsigned p;
    // while (SLOTS[SLOTS[p].next].data != val && SLOTS[p].next != 0)
    // {
    //     p = SLOTS[p].next;
    // }

    // std::cout<<"p: "<<p<<std::endl;

    // if (SLOTS[p].next==0)
    // {
    //     return 0;
    // }
    // SLOTS[p].next = SLOTS[SLOTS[p].next].next;
    // slotFree(SLOTS[p].next);
    
    // return 1;

    //simillar to deleting an element from LL

    unsigned p = head;
    unsigned t = SLOTS[head].next;
    while( (t != 0) )
    { 
        if (SLOTS[t].data == val)
        {
            SLOTS[p].next = SLOTS[t].next;
            slotFree(t);
            return 1;
        }
        p = t;
        t = SLOTS[t].next;

    }

    if (SLOTS[p].data == val) // last element

    {
        slotFree(t);
        return 1;
    }

    return 0;
}



bool clIsEmpty(unsigned head)
{
    // TODO: Implement.
    if (SLOTS[head].next == 0)
    {
        return 1;
    }
    
    return 0;
    
}

unsigned clLength(unsigned head)
{
    // TODO: Implement.
    unsigned t,count = 0;
    t = SLOTS[head].next;
    while(t != 0)
    {   
        t = SLOTS[t].next;
        count++;
    }

    return count;
    
}

unsigned clDeleteAll(unsigned head, int val)
{
    // TODO: Implement.
    // unsigned t;
    // t = SLOTS[head].next;
    // while( (t != 0) && SLOTS[t].data != val )
    // {
    //     t = SLOTS[t].next;
    // }
    // unsigned nodes_deleted = 0;
    // if (t != 0)
    // {
    //     t = SLOTS[head].next;
    //     while( (t != 0) )
    //     {
    //         slotFree(t);
    //         t = SLOTS[t].next;
    //         nodes_deleted++;
    //     }
    // }
    
    // return nodes_deleted;

        // unsigned t;
    // t = SLOTS[head].next;
    // while( (t != 0) )
    // {
    //     if (SLOTS[t].data == val)
    //     {
    //         clDelete(t,val);
    //         nodes_deleted++;
    //     }
        
    //     t = SLOTS[t].next;
    // }

    // unsigned p = head;
    // unsigned t = SLOTS[head].next;
    // while( (t != 0) )
    // { 
    //     if (SLOTS[t].data == val)
    //     {
    //         SLOTS[p].next = SLOTS[t].next;
    //         slotFree(t);
    //         nodes_deleted++;
    //     }
    //     p = t;
    //     t = SLOTS[t].next;

    // }

    unsigned nodes_deleted = 0;
    unsigned p = head;
    unsigned t = SLOTS[p].next;
    while (SLOTS[p].next != 0)
    {
        if (SLOTS[t].data == val)
        {
            SLOTS[p].next = SLOTS[t].next;     // reused clDelete code
            slotFree(t);
            t = SLOTS[p].next;
            nodes_deleted++;
        }
        else
        {                                       // if val is not deleted, no need to call SlotFree
        p = t;
        t = SLOTS[t].next;
        }
        
    }
    if (SLOTS[p].data == val) // last element

    {
        slotFree(p);
        nodes_deleted++;
    }
    
    return nodes_deleted;

    
}

unsigned clCopy(unsigned head)
{
    // TODO: Implement.
    //unsigned len = clLength(head);
    unsigned l = clCreate();
    // int len_free = freeListLen();  // thgt of calculating len of free list? try and catch easier
    unsigned t = SLOTS[head].next;
    while( (t != 0) )
    { 
        try
        {
            clInsert(l,SLOTS[t].data);
        }
        catch(...)
        {
            clDestroy(l);
        }
        
        
        t = SLOTS[t].next;
    }

    return l;
    
       
}

// used try and catch method for error checking and handling
// https://www.w3schools.com/cpp/cpp_exceptions.asp#:~:text=C%2B%2B%20try%20and%20catch&text=The%20try%20statement%20allows%20you,while%20it%20is%20being%20executed.&text=The%20catch%20statement%20allows%20you,occurs%20in%20the%20try%20block.
// handle any exceptions

void clReverse(unsigned head)
{
//     // TODO: Implement.
//     unsigned t,p;
//     t = SLOTS[head].next;
//     if (t != 0)
//     {
//         while( (SLOTS[t].next != 0) )
//         {
//         t = SLOTS[t].next;
//         }

//     }
    
    
//     p = t;
//     t = SLOTS[head].next;
//     int temp;
//     while( t != 0 )
//     {
//         temp = SLOTS[t].data;

//         t = SLOTS[t].next;
//     }

    unsigned t,p,c;
    c = SLOTS[head].next;

    while (c != 0)
    {
        t = SLOTS[c].next;
        SLOTS[c].next = p;

        p = c;
        c = t;
    }
    head = p;
}

// simillar to reversing a LL
