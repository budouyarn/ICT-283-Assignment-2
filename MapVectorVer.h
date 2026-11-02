#ifndef MAP_H
#define MAP_H
// Same public interface as Map.h, but the key/value pairs live in a Vector (kept in key order).
#include "Vector.h"

template <class Key, class Value>
class Map
{
    public:
        Map();
        bool Insert( const Key & key, const Value & value );
        Value * Find( const Key & key );
        const Value * Find( const Key & key ) const;
        bool Delete( const Key & key );
        int Size() const;
        Vector<Key> Keys() const;

    private:
        struct Entry
        {
            Key key;
            Value value;
            Entry() : key(), value() {}
            Entry( const Key & k, const Value & v ) : key( k ), value( v ) {}
        };
        Vector<Entry> m_entries;
        int Position( const Key & key, bool & present ) const;   // where key is, or where it would go
};

template <class Key, class Value>
Map<Key, Value>::Map() : m_entries() {}

template <class Key, class Value>
int Map<Key, Value>::Position( const Key & key, bool & present ) const
{
    int pos = 0;
    while (pos < m_entries.Size() && m_entries[pos].key < key)
        pos++;
    present = pos < m_entries.Size() && !(key < m_entries[pos].key);
    return pos;
}

template <class Key, class Value>
bool Map<Key, Value>::Insert( const Key & key, const Value & value )
{
    bool present = false;
    int pos = Position( key, present );
    if (present)
        return false;
    return m_entries.Insert( pos, Entry( key, value ) );
}

template <class Key, class Value>
Value * Map<Key, Value>::Find( const Key & key )
{
    bool present = false;
    int pos = Position( key, present );
    return present ? &m_entries[pos].value : nullptr;
}

template <class Key, class Value>
const Value * Map<Key, Value>::Find( const Key & key ) const
{
    bool present = false;
    int pos = Position( key, present );
    return present ? &m_entries[pos].value : nullptr;
}

template <class Key, class Value>
bool Map<Key, Value>::Delete( const Key & key )
{
    bool present = false;
    int pos = Position( key, present );
    return present && m_entries.Delete( pos );
}

template <class Key, class Value>
int Map<Key, Value>::Size() const { return m_entries.Size(); }

template <class Key, class Value>
Vector<Key> Map<Key, Value>::Keys() const
{
    Vector<Key> keys;
    for (int i = 0; i < m_entries.Size(); i++)
        keys.Insert( keys.Size(), m_entries[i].key );
    return keys;
}

#endif
