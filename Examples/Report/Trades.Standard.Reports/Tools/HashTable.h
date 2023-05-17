//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include <type_traits>
//+------------------------------------------------------------------+
//| The default allocator for the hash                               |
//| anything less than or equal to TSize is allocated via blocks     |
//| in the list,  everything else via VirtualAlloc                   |
//+------------------------------------------------------------------+
template <UINT32 TSize>
class CHashAllocator
  {
private:
   constexpr static UINT32 C_ALIGNED_SIZE=(TSize + 7) & ~7;
   //--- check the sizes
                     static_assert(C_ALIGNED_SIZE < 32 * 1024, "Too big allocator item sizes");
   //--- block header
   struct alignas(8) BlockHeader
     {
      BlockHeader      *next;
     };
   //--- free element
   struct ListItem
     {
      ListItem         *next;
     };

   BlockHeader      *m_block=nullptr;         // list of blocks
   ListItem         *m_first=nullptr;         // the first free item in the list (this is one of the items in the block)
   UINT32            m_granularity=0;         // alignment

public:
   //+------------------------------------------------------------------+
   //| Constructor                                                      |
   //+------------------------------------------------------------------+
   CHashAllocator()
     {
      SYSTEM_INFO si={};
      GetSystemInfo(&si);
      m_granularity=si.dwAllocationGranularity;
     }
   //+------------------------------------------------------------------+
   //| Destructor                                                       |
   //+------------------------------------------------------------------+
   ~CHashAllocator()
     {
      Clear();
     }
   //+------------------------------------------------------------------+
   //| Allocation                                                       |
   //+------------------------------------------------------------------+
   void* Allocate(size_t size)
     {
      //--- IMPORTANT, you cannot replace VirtualAlloc with anything else here, you need m_granularity alignment
      //--- anything larger than TSize directly
      if(size > C_ALIGNED_SIZE)
         return(VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
      //--- If there are no free ones, we allocate
      if(m_first==nullptr)
        {
         BlockHeader *new_block=(BlockHeader*)VirtualAlloc(nullptr, m_granularity, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
         if(new_block==nullptr)
            return(nullptr);
         //--- add to the list of blocks
         new_block->next=m_block;
         m_block=new_block;

         BYTE *mem=(BYTE*)new_block + sizeof(BlockHeader);
         BYTE *mem_max=(BYTE*)new_block + m_granularity;
         //--- build a list of free ones in a new block
         for(; mem + C_ALIGNED_SIZE<=mem_max; mem+=C_ALIGNED_SIZE)
           {
            ((ListItem*)mem)->next=m_first;
            m_first=((ListItem*)mem);
           }
        }
      //--- take 1 item
      void *temp=m_first;
      m_first=m_first->next;
      //---
      return(temp);
     }
   //+------------------------------------------------------------------+
   //| Free                                                             |
   //+------------------------------------------------------------------+
   void Free(void* ptr)
     {
      if(ptr==nullptr)
         return;
      //--- checking what we are freeing
      if((reinterpret_cast<uintptr_t>(ptr)&(m_granularity-1))!=0)
        {
         ((ListItem*)ptr)->next=m_first;
         m_first=(ListItem*)ptr;
        }
      else
         VirtualFree(ptr, 0, MEM_RELEASE);
     }
   //+------------------------------------------------------------------+
   //| Clearing, it is assumed that all blocks have already been freed  |
   //+------------------------------------------------------------------+
   void Clear()
     {
      m_first=nullptr;
      //--- go through all the blocks and release
      while(m_block)
        {
         void *free_mem=m_block;
         m_block=m_block->next;
         VirtualFree(free_mem, 0, MEM_RELEASE);
        }
     }
  };
//+------------------------------------------------------------------+
//| Hash table with collision resolution via list                    |
//| no duplicates                                                    |
//| all previous references are valid after addition/deletion        |
//| no duplicates, checks for duplicates                             |
//+------------------------------------------------------------------+
template <typename TValue, typename THash, typename TEqual, typename TAllocator=CHashAllocator<sizeof(TValue)+16>>
class CHashTable
  {
private:
   //--- this structure is followed by TValue
   struct HashItem
     {
      HashItem         *next;             // the next element with the same hash (to resolve collisions)
      UINT64            hash;             // "raw" hash
     };

   TAllocator        m_allocator;         // allocator
   const THash       m_hash;              // hash function
   const TEqual      m_equal;             // equivalence function

   HashItem        **m_table =nullptr;    // our hash table
   UINT64            m_count =0;          // current hash table size
   UINT64            m_filled=0;          // number of elements (all including collisions)
   UINT64            m_mask  =0;          // current mask calculated in advance

public:
   //+------------------------------------------------------------------+
   //| Constructor                                                      |
   //+------------------------------------------------------------------+
   explicit CHashTable(void)
     {
     }
   //+------------------------------------------------------------------+
   //| Constructor                                                      |
   //+------------------------------------------------------------------+
   explicit CHashTable(const TAllocator &allocator) : m_allocator(allocator)
     {
     }
   //+------------------------------------------------------------------+
   //| Destructor                                                       |
   //+------------------------------------------------------------------+
   ~CHashTable(void)
     {
      Clear();
     }
   //+------------------------------------------------------------------+
   //| Allocator                                                        |
   //+------------------------------------------------------------------+
   TAllocator& Allocator()
     {
      return(m_allocator);
     }
   //+------------------------------------------------------------------+
   //| Inserting an element in the hash and creating an instance of     |
   //| the TOther class                                                 |
   //+------------------------------------------------------------------+
   template <typename TOther=TValue, typename... TArgs>
   TOther* Insert(TArgs&&... args)
     {
      return(InsertEx<TOther>(0, std::forward<TArgs>(args)...));
     }
   //+------------------------------------------------------------------+
   //| Inserting an element in the hash creating an instance of         |
   //| the TValue class                                                 |
   //+------------------------------------------------------------------+
   template <typename TOther=TValue, typename... TArgs>
   TOther* InsertEx(UINT32 extra_size, TArgs&&... args)
     {
      //--- if rebuild is required
      if(m_filled==m_count)
         if(!RebuildTable(m_count<=0 ? 512 : m_count * 2))
            return(nullptr);
      //--- allocate
      HashItem *item=(HashItem*)m_allocator.Allocate(sizeof(HashItem) + sizeof(TOther) + extra_size);
      if(item==nullptr)
         return(nullptr);
      TOther* obj=(TOther*)(item + 1);
      //--- construct
      new (obj) TOther(std::forward<TArgs>(args)...);
      //--- now look for where to put
      item->hash=m_hash(*obj);
      UINT64 idx=item->hash & m_mask;
      //--- check for duplicates
      HashItem *temp=m_table[idx];
      while(temp)
        {
         if(m_equal(*(TValue*)(temp + 1), *obj))
           {
            obj->~TValue();
            //--- we have a duplicate
            m_allocator.Free(item);
            return(nullptr);
           }
         temp=temp->next;
        }
      //--- no duplicates, insert
      item->next=m_table[idx];
      m_table[idx]=item;
      m_filled++;
      //---
      return(obj);
     }
   //+------------------------------------------------------------------+
   //| Search for the element                                           |
   //+------------------------------------------------------------------+
   template <typename... TArgs>
   TValue* Find(TArgs&&... args) const
     {
      if(m_count<=0)
         return(nullptr);
      //--- calculate hash
      UINT64 hash=m_hash(std::forward<TArgs>(args)...);
      //--- now the equivalence
      HashItem *temp=m_table[hash & m_mask];
      while(temp)
        {
         if(m_equal(*(TValue*)(temp + 1), std::forward<TArgs>(args)...))
           {
            //--- found
            return((TValue*)(temp + 1));
           }
         temp=temp->next;
        }
      //--- nothing found
      return(nullptr);
     }
   //+------------------------------------------------------------------+
   //| Delete the 1st and only the 1st element                          |
   //+------------------------------------------------------------------+
   template <typename... TArgs>
   bool Remove(TArgs&&... args)
     {
      if(m_count<=0)
         return(false);
      //--- calculate hash
      UINT64 hash=m_hash(std::forward<TArgs>(args)...);
      //--- now the equivalence
      HashItem **temp=&m_table[hash & m_mask];
      while(*temp)
        {
         if(m_equal(*(TValue*)(*temp + 1), std::forward<TArgs>(args)...))
           {
            //--- correct the list
            HashItem *to_delete=*temp;
            *temp=to_delete->next;
            //--- found deleted
            ((TValue*)(to_delete + 1))->~TValue();
            m_allocator.Free(to_delete);
            m_filled--;
            //---
            return(true);
           }
         temp=&(*temp)->next;
        }
      //--- nothing found
      return(false);
     }
   //+------------------------------------------------------------------+
   //| Preparing a table of a predetermined size                        |
   //+------------------------------------------------------------------+
   bool Reserve(UINT64 count)
     {
      UINT64 mult=1;
      //--- calculate the power of two
      while(count > mult)
         mult*=2;
      //--- rebuild the table
      return(RebuildTable(mult));
     }
   //+------------------------------------------------------------------+
   //| Sweep                                                            |
   //+------------------------------------------------------------------+
   void Clear(void)
     {
      //--- free memory
      if(m_table)
        {
         //--- pass through all the elements
         for(UINT64 tt=0; tt < m_count; tt++)
           {
            HashItem *temp=m_table[tt];
            while(temp)
              {
               HashItem *next=temp->next;
               //--- delete
               ((TValue*)(temp + 1))->~TValue();
               m_allocator.Free(temp);
               temp=next;
              }
           }
         //---
         m_allocator.Free(m_table);
         m_table=nullptr;
        }
      //---
      m_count =0;
      m_filled=0;
      m_mask  =0;
     }
   //+------------------------------------------------------------------+
   //| Iterate over each element in the hash,                           |
   //| through the passed functor                                       |
   //+------------------------------------------------------------------+
   template <typename TIterator, typename... TArgs>
   void Iterate(TIterator &&iterator, TArgs&&... args)
     {
      for(UINT64 tt=0; tt < m_count; tt++)
        {
         HashItem *temp=m_table[tt];
         while(temp)
           {
            HashItem* next=temp->next;
            //--- pass the call to the functor with additional parameters
            if(!iterator(*(TValue*)(temp + 1), std::forward<TArgs>(args)...))
               return;
            //--- to the next
            temp=next;
           }
        }
     }
   //+------------------------------------------------------------------+
   //| Number of elements                                               |
   //+------------------------------------------------------------------+
   UINT64 Size(void) const
     {
      return(m_filled);
     }

private:
   //--- prohibit copy constructor and assignment
                     CHashTable(const CHashTable&)=delete;
   const CHashTable& operator=(const CHashTable&)=delete;
   //+------------------------------------------------------------------+
   //| Rebuild the tree                                                 |
   //+------------------------------------------------------------------+
   bool RebuildTable(UINT64 new_count)
     {
      if(new_count<=m_count)
         return(false);
      //--- allocate memory for the new table
      HashItem **new_table=(HashItem**)m_allocator.Allocate(sizeof(HashItem*) * new_count);
      if(new_table==nullptr)
         return(false);
      //--- be sure to clean up
      ZeroMemory(new_table, sizeof(HashItem*) * new_count);

      UINT64 new_mask=new_count - 1;
      //--- if there is something to copy
      if(m_table)
        {
         //--- now copy and paste the old stuff
         for(UINT64 tt=0; tt < m_count; tt++)
           {
            HashItem* temp=m_table[tt];
            while(temp)
              {
               //--- save the next
               HashItem* next=temp->next;
               //--- insert into the new table
               UINT64 idx=temp->hash & new_mask;
               temp->next=new_table[idx];
               new_table[idx]=temp;
               //--- to the next
               temp=next;
              }
           }
         //--- release old
         m_allocator.Free(m_table);
        }
      //--- swap tables
      m_table=new_table;
      m_mask =new_mask;
      m_count=new_count;
      //---
      return(true);
     }
  };
//+------------------------------------------------------------------+
