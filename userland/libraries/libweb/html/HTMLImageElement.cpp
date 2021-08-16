/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libgfx/Bitmap.h>
#include <libweb/css/Parser/Parser.h>
#include <libweb/css/StyleResolver.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Event.h>
#include <libweb/html/EventNames.h>
#include <libweb/html/HTMLImageElement.h>
#include <libweb/layout/ImageBox.h>
#include <libweb/loader/ResourceLoader.h>

namespace Web::HTML {

HTMLImageElement::HTMLImageElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
    , m_image_loader(*this)
{
    m_image_loader.on_load = [this] {
        this->document().update_layout();
        dispatch_event(DOM::Event::create(EventNames::load));
    };

    m_image_loader.on_fail = [this] {
        dbgln("HTMLImageElement: Resource did fail: {}", src());
        this->document().update_layout();
        dispatch_event(DOM::Event::create(EventNames::error));
    };

    m_image_loader.on_animate = [this] {
        if (layout_node())
            layout_node()->set_needs_display();
    };
}

HTMLImageElement::~HTMLImageElement()
{
}

void HTMLImageElement::apply_presentational_hints(CSS::StyleProperties& style) const
{
    for_each_attribute([&](auto& name, auto& value) {
        if (name == HTML::AttributeNames::width) {
            if (auto parsed_value = parse_html_length(document(), value)) {
                style.set_property(CSS::PropertyID::Width, parsed_value.release_nonnull());
            }
        } else if (name == HTML::AttributeNames::height) {
            if (auto parsed_value = parse_html_length(document(), value)) {
                style.set_property(CSS::PropertyID::Height, parsed_value.release_nonnull());
            }
        }
    });
}

void HTMLImageElement::parse_attribute(const FlyString& name, const String& value)
{
    HTMLElement::parse_attribute(name, value);

    if (name == HTML::AttributeNames::src && !value.is_empty())
        m_image_loader.load(document().complete_url(value));
}

RefPtr<Layout::Node> HTMLImageElement::create_layout_node()
{
    auto style = document().style_resolver().resolve_style(*this);
    if (style->display() == CSS::Display::None)
        return nullptr;
    return adopt_ref(*new Layout::ImageBox(document(), *this, move(style), m_image_loader));
}

const Gfx::Bitmap* HTMLImageElement::bitmap() const
{
    return m_image_loader.bitmap(m_image_loader.current_frame_index());
}

}