/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/RefPtr.h>
#include <libweb/dom/Node.h>
#include <libweb/Forward.h>

namespace Web::DOM {

class Position {
public:
    Position() { }
    Position(Node&, unsigned offset);

    ~Position();

    bool is_valid() const { return m_node; }

    Node* node() { return m_node; }
    const Node* node() const { return m_node; }

    unsigned offset() const { return m_offset; }
    bool offset_is_at_end_of_node() const;
    void set_offset(unsigned value) { m_offset = value; }
    bool increment_offset();
    bool decrement_offset();

    bool operator==(const Position& other) const
    {
        return m_node == other.m_node && m_offset == other.m_offset;
    }

    bool operator!=(const Position& other) const
    {
        return !(*this == other);
    }

    String to_string() const;

private:
    RefPtr<Node> m_node;
    unsigned m_offset { 0 };
};

}

namespace Base {
template<>
struct Formatter<Web::DOM::Position> : Formatter<StringView> {
    void format(FormatBuilder& builder, const Web::DOM::Position& value)
    {
        Formatter<StringView>::format(builder, value.to_string());
    }
};

}