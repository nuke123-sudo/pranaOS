/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/ByteBuffer.h>
#include <libgfx/Forward.h>
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLCanvasElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLCanvasElementWrapper;

    HTMLCanvasElement(DOM::Document&, QualifiedName);
    virtual ~HTMLCanvasElement() override;

    const Gfx::Bitmap* bitmap() const { return m_bitmap; }
    Gfx::Bitmap* bitmap() { return m_bitmap; }
    bool create_bitmap();

    CanvasRenderingContext2D* get_context(String type);

    unsigned width() const;
    unsigned height() const;

    String to_data_url(const String& type, Optional<double> quality) const;

private:
    virtual RefPtr<Layout::Node> create_layout_node() override;

    RefPtr<Gfx::Bitmap> m_bitmap;
    RefPtr<CanvasRenderingContext2D> m_context;
};

}