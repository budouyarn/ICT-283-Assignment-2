#ifndef MAP_H
#define MAP_H
// Same public interface as Map.h, but the key/value pairs live in a Vector (kept in key order).
#include "Vector.h"

template <class K, class V>
class Map
{
    public:
        Map();
        bool Insert( const K & key, const V & value );
        V * Find( const K & key );
        const V * Find( const K & key ) const;
        bool Delete( const K & key );
        int Size() const;
        Vector<K> Keys() const;

    private:
        struct Entry
        {
            K key;
            V value;
            Entry() : key(), value() {}
            Entry( const K & k, const V & v ) : key( k ), value( v ) {}
        };
        Vector<Entry> m_entries;
        int Position( const K & key, bool & present ) const;   // where key is, or where it would go
};

template <class K, class V>
Map<K, V>::Map() : m_entries() {}

template <class K, class V>
int Map<K, V>::Position( const K & key, bool & present ) const
{
    int pos = 0;
    while (pos < m_entries.Size() && m_entries[pos].key < key)
        pos++;
    present = pos < m_entries.Size() && !(key < m_entries[pos].key);
    return pos;
}

template <class K, class V>
bool Map<K, V>::Insert( const K & key, const V & value )
{
    bool present = false;
    int pos = Position( key, present );
    if (present)
        return false;
    return m_entries.Insert( pos, Entry( key, value ) );
}

template <class K, class V>
V * Map<K, V>::Find( const K & key )
{
    bool present = false;
    int pos = Position( key, present );
    return present ? &m_entries[pos].value : nullptr;
}

template <class K, class V>
const V * Map<K, V>::Find( const K & key ) const
{
    bool present = false;
    int pos = Position( key, present );
    return present ? &m_entries[pos].value : nullptr;
}

template <class K, class V>
bool Map<K, V>::Delete( const K & key )
{
    bool present = false;
    int pos = Position( key, present );
    return present && m_entries.Delete( pos );
}

template <class K, class V>
int Map<K, V>::Size() const { return m_entries.Size(); }

template <class K, class V>
Vector<K> Map<K, V>::Keys() const
{
    Vector<K> keys;
    for (int i = 0; i < m_entries.Size(); i++)
        keys.Insert( keys.Size(), m_entries[i].key );
    return keys;
}

#endif
