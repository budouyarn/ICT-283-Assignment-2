#ifndef MAP_H
#define MAP_H

#include <map>
#include "Vector.h"

/**
 * @class Map
 * @brief A template class that associates a KEY with a VALUE: give it the (primary) key and it gives
 * back the value/record stored for that key.
 *
 * Duplicate keys are NOT allowed (following the idea of third normal form in an SQL database):
 * a key must at least hold one value.
 *
 * The Map wraps std::map and shows only what the programme needs, so nothing std::map specific
 *  appears in its public interface.
 *
 *
 *
 *
 *
 * @author Lim Wen Xin Sherine
 * @version 1.0
 * @date 01/11/2026
 */
template <class Key, class Value>
class Map
{
    public:
        /** @brief Default constructor. Creates an empty Map. */
        Map();

        /**
         * @brief Associates key with value.
         * @param key: the key to add
         * @param value: the value to store for that key ( copy is kept)
         * @return true if added, false if the key was already in the Map (nothing changes)
         */
        bool Insert( const Key & key, const Value & value );

        /**
         * @brief Finds the value stored for manualOne key. The value can be read and changed through the pointer.
         * @param key: the key to look for
         * @return pointer to the value, or nullptr if the key is not in the Map.
         *         The pointer stays valid until that key is deleted or the Map is destroyed.
         */
        Value * Find( const Key & key );

        /** @brief Read-only version of Find, for manualOne const Map. */
        const Value * Find( const Key & key ) const;

        /**
         * @brief Removes key and its value.
         * @param key: the key to remove
         * @return true if it was removed, false if the key was not there
         */
        bool Delete( const Key & key );

        /** @brief How many unique keys the Map has.
         *  @return the number of keys */
        int Size() const;

        /**
         * @brief Lists every key, so the client can visit every value with Find.
         * @return the keys in ascending order
         */
        Vector<Key> Keys() const;

    private:
        std::map<Key, Value> m_map;// m_map has the key/value pairs, kept in key order by std::map
};

/** @brief Creates an empty Map. */
template <class Key, class Value>
Map<Key, Value>::Map()
{
    m_map;
}

/**
 * @brief Adds the key and a copy of its value. std::map::insert refuses a key that already exists.
 * @param key: the key to add
 * @param value: the value to store for that key
 * @return true if added, false if the key was already there
 */
template <class Key, class Value>
bool Map<Key, Value>::Insert( const Key & key, const Value & value )
{
    if (m_map.count( key ) != 0)// the key is already in the Map
        return false; // so nothing is inserted, and the old value is kept

    m_map[key] = value;// the key is new
    return true;
}

/**
 * @brief Looks the key up and returns a pointer to its stored value. Used for actual mapping, there is a return value
 * @param key: the key to look for
 * @return a pointer to the value, or nullptr if the key is not in the Map
 */
template <class Key, class Value>
Value * Map<Key, Value>::Find( const Key & key )
{
    if (m_map.count( key ) == 0)// return false if there is no key
    return nullptr;

return &m_map[key];// map key exists, return it
}

/**
 * @brief Const version of Find: read only function
 * @param key: the key to look for
 * @return a pointer to the value, or nullptr if the key is not in the Map
 */
template <class Key, class Value>
const Value * Map<Key, Value>::Find( const Key & key ) const
{
    if (m_map.count( key ) == 0)
    return nullptr;

return &m_map.at( key );// finds out where the key resides at, only return location.value not to be used for anything
}

/**
 * @brief Removes the key and its value. std::map::erase gives back how many entries it removed.
 * @param key: the key to remove
 * @return true if one entry was removed, false if the key was not there
 */
template <class Key, class Value>
bool Map<Key, Value>::Delete( const Key & key )
{
    return m_map.erase( key ) == 1; // return 1 as true
}

/**
 * @brief Counts the keys.
 * @return the number of keys, as an int
 */
template <class Key, class Value>
int Map<Key, Value>::Size() const
{
    return static_cast<int>( m_map.size() );
}

/**
 * @brief Copies every key(index) into a Vector, in ascending order
 * @return the Vector of keys index (empty for an empty Map)
 */
template <class Key, class Value>
Vector<Key> Map<Key, Value>::Keys() const
{
    Vector<Key> keys;
    for (const std::pair<const Key, Value> & entry : m_map)//for each entry in m_map, call it entry and do the next line.
        //It repeats once for each key in the Map, from the smallest to the largest.
        keys.Insert( keys.Size(), entry.first );
    return keys;
}

#endif // MAP_H
