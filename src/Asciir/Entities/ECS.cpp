#include "arpch.h"
#include "ECS.h"

namespace Asciir
{

    /*
    * ComponentBuffer definitions
    */

    uint8_t* ComponentBuffer::get(size_t indx)
    {
        AR_ASSERT_MSG(hasIndex(indx), "Invalid index was passed: ", indx, "\nSize: ", getSize(), "\nData length", dataLength());
        return &m_data[mapIncoming(indx) * (m_data_size + sizeof(size_t))];
    }

    void ComponentBuffer::set(size_t indx, uint8_t* data)
    {
        if (!hasIndex(indx))
        {
            m_data.reserve(m_data.size() + m_data_size + sizeof(size_t));

            m_data.insert(m_data.end(), data, data + m_data_size);
            m_data.insert(m_data.end(), (uint8_t*) &indx, (uint8_t*) &indx + sizeof(size_t));

            m_sparse_map[indx] = dataLength() - 1;
        }
        else
        {
            auto begin = m_data.begin() + mapIncoming(indx) * (m_data_size + sizeof(size_t));
            std::copy(data, data + m_data_size, begin);
        }
    }

    // assumes all elements outside of the new range has been removed
    // if these should automatticly be removed, call resize instead.
    void ComponentBuffer::setSize(size_t size)
    {
    #ifdef AR_DEBUG
        for (size_t i = size; i < getSize(); i++)
            AR_ASSERT_MSG(!hasIndex(i), "setSize was called with elements outside the new size range\nNew size: ", size, "\nCurrent size: ", getSize(), "Element index: ", i);
    #endif

        m_sparse_map.resize(size);
    }

    void ComponentBuffer::resize(size_t size)
    {
        for (size_t i = size; i < getSize(); i++)
            if(hasIndex(i)) remove(i);
        
        setSize(size);
    }

    bool ComponentBuffer::hasIndex(size_t indx)
    {
        for (size_t i = 0; i < dataLength(); i++)
            if (mapOutgoing(i) == indx) return true;
        
        return false;
    }

    size_t ComponentBuffer::mapOutgoing(size_t indx)
    {
        // go to the start of the target component and offset by component size to get the index relative to the sparse map
        return *(size_t*) &m_data[indx * (m_data_size + sizeof(size_t)) + m_data_size];
    }

    void ComponentBuffer::remove(size_t indx)
    {
        // offset all indexes past the element to the left in the sparse map
        for (size_t i = mapIncoming(indx) + 1; i < dataLength(); i++)
            m_sparse_map[mapOutgoing(i)]--;
         
        // remove the component from component data vector
        auto begin = m_data.begin() + mapIncoming(indx) * (m_data_size + sizeof(size_t));
        auto end = begin + (m_data_size + sizeof(size_t));
        m_data.erase(begin, end);
    }
    
    /*
    * System definitions
    */

    void System::run()
    {
        for (Ref<Scene> scene : m_system_scenes)
        {


        }
    }

    void System::addScene(Ref<Scene> scene)
    {
    }

}
