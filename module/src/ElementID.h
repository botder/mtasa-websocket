/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        WebsocketID.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <cstdint>

class ElementID
{
    static constexpr std::size_t IndexBits = 16;
    static constexpr std::size_t IndexMask = (1 << IndexBits) - 1;

    static constexpr std::size_t GenerationBits = 8;
    static constexpr std::size_t GenerationMask = (1 << GenerationBits) - 1;

public:
    static constexpr std::uint32_t Invalid = -1;

    ElementID()
        : m_ID(Invalid)
    {
    }

    constexpr ElementID(std::uint32_t id)
        : m_ID(id)
    {
    }

    constexpr ElementID(std::size_t index, std::size_t generation)
        : m_ID(((generation & GenerationMask) << IndexBits) | (index & IndexMask))
    {
    }

    constexpr std::uint8_t GetGeneration() const
    {
        return (m_ID >> IndexBits) & GenerationMask;
    }

    constexpr std::uint16_t GetIndex() const
    {
        return m_ID & IndexMask;
    }

    constexpr operator std::uint32_t()
    {
        return m_ID;
    }

private:
    std::uint32_t m_ID;
};
