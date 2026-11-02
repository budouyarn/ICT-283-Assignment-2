#ifndef MAP_H
#define MAP_H

#include <map>
#include "Vector.h"

/**
 * A Map associates a KEY with a VALUE: give it a key and it gives back the value stored for that key.
 *
 * Duplicate keys are NOT allowed. a key has at most one value. Insert refuses a key that is already
 * there.
 *
 *
 *
 * @tparam Key the type of the key
 * @tparam Value the type of the value
 *
 * @class Map
 * @author Lim Wen Xin Sherine
 * @version 1.0
 * @date    01/11/2026
 */
template <class Key, class Value>
class Map
{
    public:
        /** @brief Default constructor. Creates an empty Map. */
        Map();

        /**
         * @brief Associates a key with a value.
         * @param key: the key to add
         * @param value: the value to store for that key (a copy is kept)
         * @return true if added, false if the key was already in the Map (nothing changes)
         */
        bool Insert( const Key & key, const Value & value );

        /**
         * @brief Finds the value stored for a key. The value can be read and changed through the pointer.
         * @param key: the key to look for
         * @return a pointer to the value, or nullptr if the key is not in the Map.
         *         The pointer stays valid until that key is deleted or the Map is destroyed.
         */
        Value * Find( const Key & key );

        /** @brief Read-only version of Find, for a const Map. */
        const Value * Find( const Key & key ) const;

        /**
         * @brief Removes a key and its value.
         * @param key: the key to remove
         * @return true if it was removed, false if the key was not there
         */
        bool Delete( const Key & key );

        /** @brief How many keys the Map holds.
         *  @return the number of keys */
        int Size() const;

        /**
         * @brief Lists every key, so the client can visit every value with Find.
         * @return the keys in ascending order
         */
        Vector<Key> Keys() const;

    private:
        std::map<Key, Value> m_map;  // the key/value pairs, kept in key order by std::map */
};

template <class Key, class Value>
Map<Key, Value>::Map()
{
    m_map=0;
}

template <class Key, class Value>
bool Map<Key, Value>::Insert( const Key & key, const Value & value )
{
    return m_map.insert( std::pair<const Key, Value>( key, value ) ).second;   // .second is false if the key was already there
}

template <class Key, class Value>
Value * Map<Key, Value>::Find( const Key & key )
{
    typename std::map<Key, Value>::iterator found = m_map.find( key );
    if (found == m_map.end())
        return nullptr;
    return &found->second;
}

template <class Key, class Value>
const Value * Map<Key, Value>::Find( const Key & key ) const
{
    typename std::map<Key, Value>::const_iterator found = m_map.find( key );
    if (found == m_map.end())
        return nullptr;
    return &found->second;
}

template <class Key, class Value>
bool Map<Key, Value>::Delete( const Key & key )
{
    return m_map.erase( key ) == 1;
}

template <class Key, class Value>
int Map<Key, Value>::Size() const
{
    return static_cast<int>( m_map.size() );
}

template <class Key, class Value>
Vector<Key> Map<Key, Value>::Keys() const
{
    Vector<Key> keys;
    for (typename std::map<Key, Value>::const_iterator it = m_map.begin(); it != m_map.end(); ++it)
        keys.Insert( keys.Size(), it->first );
    return keys;
}

#endif // MAP_H
