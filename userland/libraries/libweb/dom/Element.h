/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <base/String.h>
#include <libweb/css/CSSStyleDeclaration.h>
#include <libweb/css/StyleResolver.h>
#include <libweb/dom/Attribute.h>
#include <libweb/dom/ExceptionOr.h>
#include <libweb/dom/NonDocumentTypeChildNode.h>
#include <libweb/dom/ParentNode.h>
#include <libweb/html/AttributeNames.h>
#include <libweb/html/TagNames.h>
#include <libweb/layout/Node.h>
#include <libweb/QualifiedName.h>

namespace Web::DOM {

class Element
    : public ParentNode
    , public NonDocumentTypeChildNode<Element> {

public:
    using WrapperType = Bindings::ElementWrapper;

    Element(Document&, QualifiedName);
    virtual ~Element() override;

    const String& qualified_name() const { return m_qualified_name.as_string(); }
    const String& html_uppercased_qualified_name() const { return m_html_uppercased_qualified_name; }
    virtual FlyString node_name() const final { return html_uppercased_qualified_name(); }
    const FlyString& local_name() const { return m_qualified_name.local_name(); }

    const String& tag_name() const { return html_uppercased_qualified_name(); }

    const FlyString& prefix() const { return m_qualified_name.prefix(); }
    const FlyString& namespace_() const { return m_qualified_name.namespace_(); }

    const FlyString& namespace_uri() const { return namespace_(); }

    bool has_attribute(const FlyString& name) const { return find_attribute(name) != nullptr; }
    bool has_attributes() const { return !m_attributes.is_empty(); }
    String attribute(const FlyString& name) const;
    String get_attribute(const FlyString& name) const { return attribute(name); }
    ExceptionOr<void> set_attribute(const FlyString& name, const String& value);
    void remove_attribute(const FlyString& name);

    template<typename Callback>
    void for_each_attribute(Callback callback) const
    {
        for (auto& attribute : m_attributes)
            callback(attribute.name(), attribute.value());
    }

    bool has_class(const FlyString&, CaseSensitivity = CaseSensitivity::CaseSensitive) const;
    const Vector<FlyString>& class_names() const { return m_classes; }

    virtual void apply_presentational_hints(CSS::StyleProperties&) const { }
    virtual void parse_attribute(const FlyString& name, const String& value);

    void recompute_style();

    Layout::NodeWithStyle* layout_node() { return static_cast<Layout::NodeWithStyle*>(Node::layout_node()); }
    const Layout::NodeWithStyle* layout_node() const { return static_cast<const Layout::NodeWithStyle*>(Node::layout_node()); }

    String name() const { return attribute(HTML::AttributeNames::name); }

    const CSS::StyleProperties* specified_css_values() const { return m_specified_css_values.ptr(); }
    NonnullRefPtr<CSS::StyleProperties> computed_style();

    const CSS::CSSStyleDeclaration* inline_style() const { return m_inline_style; }

    NonnullRefPtr<CSS::CSSStyleDeclaration> style_for_bindings();

    String inner_html() const;
    void set_inner_html(StringView);

    bool is_focused() const;
    virtual bool is_focusable() const { return false; }

    bool is_active() const;

    NonnullRefPtr<HTMLCollection> get_elements_by_tag_name(FlyString const&);
    NonnullRefPtr<HTMLCollection> get_elements_by_class_name(FlyString const&);

    ShadowRoot* shadow_root() { return m_shadow_root; }
    const ShadowRoot* shadow_root() const { return m_shadow_root; }
    void set_shadow_root(RefPtr<ShadowRoot>);

    Optional<CSS::StyleResolver::CustomPropertyResolutionTuple> resolve_custom_property(const String& custom_property_name)
    {
        return m_custom_properties.get(custom_property_name);
    }
    void add_custom_property(const String& custom_property_name, CSS::StyleResolver::CustomPropertyResolutionTuple style_property)
    {
        m_custom_properties.set(custom_property_name, style_property);
    }

protected:
    RefPtr<Layout::Node> create_layout_node() override;

private:
    Attribute* find_attribute(const FlyString& name);
    const Attribute* find_attribute(const FlyString& name) const;

    void make_html_uppercased_qualified_name();

    QualifiedName m_qualified_name;
    String m_html_uppercased_qualified_name;
    Vector<Attribute> m_attributes;

    RefPtr<CSS::CSSStyleDeclaration> m_inline_style;

    RefPtr<CSS::StyleProperties> m_specified_css_values;
    HashMap<String, CSS::StyleResolver::CustomPropertyResolutionTuple> m_custom_properties;

    Vector<FlyString> m_classes;

    RefPtr<ShadowRoot> m_shadow_root;
};

template<>
inline bool Node::fast_is<Element>() const { return is_element(); }

}