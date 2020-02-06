#include <cstdlib>  //This is for the exit command.
// CPP program for array implementation of queue  
#include <bits/stdc++.h> 
using namespace std; 


template <class etype>
class QueueArray
{
  public:
    QueueArray(int);
    ~QueueArray();
    int Asize(); //return size
    etype Dequeue();
    int Enqueue(const etype &item,const int index);
    int QAsize(); 
    int Qsize(int index) ;
    etype* Qstate(int index);

  private:
    int size;      //size of the array
    queue<etype>* array;   //the array of queues.  It must be an array (NO not a vector)
    int totalItems; //total number of items stored in the queues
};

//Constructor for the queue array.  It sets the default size
//to 10, initializes the private variables size and totalItems
template <class etype>
QueueArray<etype>::QueueArray(int sz = 10):size(sz),totalItems(0)
{
  array = new queue<etype>[size];
  if( array == NULL)
  {
    cout << "Not enough memory to create the array" << endl;
    exit(-1);
  }
}

template <class etype>
int QueueArray<etype>:: Asize()
{
    return size;
}

template <class etype>
int QueueArray<etype>::Enqueue(const etype &item,const int index)
{
    if(index>size || index < 0)
    {
        return -1;
    }
    else
    {
        array[index].push(item);
        if(array[index].back()==item)
        {
            return 1;}
        return 1;
    }
    
}

template <class etype>
QueueArray<etype>::~QueueArray()
{delete array;}

template <class etype>
int QueueArray<etype>::Qsize(int index)
{
   if(index>size || index < 0)
    {
        return -1;
    }
    else
        return array[index].size();
}

template <class etype>
int QueueArray<etype>::QAsize()
{
   totalItems = 0;
   for(int i =0; i<size; i++)
   {
      totalItems += array[i].size(); 
   }
   return totalItems;
}
template <class etype>
etype QueueArray<etype>::Dequeue()
{
    if(QAsize() == 0)
   {
       return 0;
   } 
   for(int i =0; i<size; i++)
   {
      if(array[i].size()>0)
      { 
          etype val = array[i].front();
          array[i].pop();
          return val;
      }
   }
}

template <class etype>
etype* QueueArray<etype>::Qstate(int index)
{
   if(index>size || index < 0)
    {
        return NULL;
    }
   
   int s = Qsize(index);
   etype* tempArray =  new etype[s];
 
   
   for(int i =0; i<s; i++)
   {
        etype val = array[index].front();
          array[index].pop();
       tempArray[i]=val;
   }
   for(int i =0; i<s; i++)
   {
       array[index].push(tempArray[i]);
   }
   
   return tempArray;
   } 