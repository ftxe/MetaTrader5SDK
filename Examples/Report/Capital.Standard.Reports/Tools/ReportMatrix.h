//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportVector.h"
//+------------------------------------------------------------------+
//| Report matrix template class                                     |
//+------------------------------------------------------------------+
template<class T>
class TReportMatrix
  {
public:
   typedef TReportVector<T> TVector;               // vector type

private:
   typedef TReportVector<TVector> TVectorVector;   // vector of vectors type

private:
   TVectorVector     m_columns;                    // columns vector
   UINT              m_height;                     // column initial height

public:
   explicit          TReportMatrix(const bool size_fixed) : m_columns(size_fixed),m_height(0) {}
                    ~TReportMatrix(void) {}
   //--- clear
   void              Clear(void);
   //--- initialization
   bool              Initialize(const UINT width,const UINT height);
   //--- dimensions
   UINT              Width(void) const                { return(m_columns.Total()); }
   UINT              Height(void) const;
   //--- columns
   TVector*          Column(const UINT col);
   const TVector*    Column(const UINT col) const     { return(m_columns.Item(col)); }
   //--- items
   T*                Item(const UINT col,const UINT row);
   const T*          Item(const UINT col,const UINT row) const;
   //--- binary operation for all columns
   template<class Func>
   bool              OperationColumns(TVector &vector,const Func &func) const;
   //--- unary operation for all columns in reverse order
   template<class Func>
   const TVector*    OperationColumnsReverse(const Func &func);
  };
//+------------------------------------------------------------------+
//| clear                                                            |
//+------------------------------------------------------------------+
template<class T>
void TReportMatrix<T>::Clear(void)
  {
   m_columns.Clear();
   m_height=0;
  }
//+------------------------------------------------------------------+
//| initialization                                                   |
//+------------------------------------------------------------------+
template<class T>
bool TReportMatrix<T>::Initialize(const UINT width,const UINT height)
  {
//--- clear
   Clear();
//--- check empty
   if(!width)
      return(true);
//--- initialize columns
   if(!m_columns.Initialize(width))
      return(false);
//--- store column initial height
   m_height=height;
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
//| matrix height                                                    |
//+------------------------------------------------------------------+
template<class T>
UINT TReportMatrix<T>::Height(void) const
  {
//--- iterate all columns
   UINT height=0;
   for(UINT col=0,columns=m_columns.Total();col<columns;col++)
      if(const TVector *column=Column(col))
        {
         //--- calculate maximum height
         const UINT total=column->Total();
         if(height<total)
            height=total;
        }
//--- return height
   return(height);
  }
//+------------------------------------------------------------------+
//| matrix column                                                    |
//+------------------------------------------------------------------+
template<class T>
typename TReportMatrix<T>::TVector* TReportMatrix<T>::Column(const UINT col)
  {
//--- get column
   TVector *column=m_columns.Item(col);
   if(!column)
      return(nullptr);
//--- check column initialized
   if(!column->Constructed())
     {
      //--- construct column
      new (column) TVector();
      //--- initialize column
      if(!column->Initialize(m_height))
         return(nullptr);
     }
//--- return column
   return(column);
  }
//+------------------------------------------------------------------+
//| matrix item                                                      |
//+------------------------------------------------------------------+
template<class T>
T* TReportMatrix<T>::Item(const UINT col,const UINT row)
  {
//--- matrix column
   TVector *column=Column(col);
   if(!column)
      return(nullptr);
//--- column item
   return(column->Item(row));
  }
//+------------------------------------------------------------------+
//| matrix item                                                      |
//+------------------------------------------------------------------+
template<class T>
const T* TReportMatrix<T>::Item(const UINT col,const UINT row) const
  {
//--- matrix column
   TVector *column=Column(col);
   if(!column)
      return(nullptr);
//--- column item
   return(column->Item(row));
  }
//+------------------------------------------------------------------+
//| Binary operation for all columns                                 |
//+------------------------------------------------------------------+
template<class T>
template<class Func>
bool TReportMatrix<T>::OperationColumns(TVector &vector,const Func &func) const
  {
//--- initialize vector with height
   const UINT height=Height();
   if(!vector.Initialize(height))
      return(false);
//--- iterate all columns and call operation for each
   for(UINT col=0,columns=m_columns.Total();col<columns;col++)
      if(const TVector *column=Column(col))
         if(!vector.Operation(*column,func))
            return(false);
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Unary operation for all columns in reverse order                 |
//+------------------------------------------------------------------+
template<class T>
template<class Func>
const typename TReportMatrix<T>::TVector* TReportMatrix<T>::OperationColumnsReverse(const Func &func)
  {
//--- iterate all columns in reverse order and call operation for each
   const TVector *column_prev=nullptr;
   for(UINT col=m_columns.Total();col>0;)
     {
      col--;
      TVector *column=Column(col);
      if(column && column_prev)
         if(!column->Operation(*column_prev,func))
            return(nullptr);
      column_prev=column;
     }
//--- return first column
   return(column_prev);
  }
//+------------------------------------------------------------------+
