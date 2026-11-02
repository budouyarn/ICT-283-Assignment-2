#ifndef VECTOR_H
#define VECTOR_H

#include <vector>// STL vector

using std::vector;

/**
 * @brief A template class that uses std::vector
 * Insert, Delete, operator[] and Size.
 * This vector is used for Stats.h to store calculated values
 * @tparam T the type of element stored
 */
template <class T>
class Vector
{
    public:
        Vector();
        Vector( int n ); // n is the initial number of elements

        bool Insert( int pos, const T & data );// Create new data
        bool Delete( int pos );// removing the element at pos
        const T & operator[]( int pos ) const;// read only access to element at pos
        T & operator[]( int pos );// read/write access at pos
        int Size() const;// reports how many elements in use

    private:
        vector<T> m_vector; // STL vector holds the elements, it manages its own memory and growth
        int m_size;// how many elements are currently in use
};

template <class T>
Vector<T>::Vector()
{
    m_vector = vector<T>();
    m_size=0;
}

template <class T>
Vector<T>::Vector( int n )
{
    m_vector = vector<T>();
    m_size=0;
    for (int counter = 0; counter < n; counter++) // runs 0 times if n is 0 or negative
    {
        m_vector.push_back( T() );
        m_size++;
    }
}

template <class T>
bool Vector<T>::Insert( int pos, const T & data )
{
    if (pos < 0 || pos > m_size)
    {
        return false;
    }
    else if (pos == m_size)
    {
        m_vector.push_back( data );
    }
    else
    {
        m_vector.insert( m_vector.begin() + pos, data );
    }
    m_size++;
    return true;
}

template <class T>
bool Vector<T>::Delete( int pos )
{
    if (pos < 0 || pos >= m_size)
        return false;

    m_vector.erase( m_vector.begin() + pos );
    m_size--;
    return true;
}

// pos must be from 0 to Size() - 1(n-1)
template <class T>
const T & Vector<T>::operator[]( int pos ) const
{
    return m_vector[pos];
}

// pos must be from 0 to Size() - 1
template <class T>
T & Vector<T>::operator[]( int pos )
{
    return m_vector[pos];
}

template <class T>
int Vector<T>::Size() const
{
    return m_size;
}

#endif
