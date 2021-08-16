/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullRefPtrVector.h>
#include <base/OwnPtr.h>
#include <libweb/css/CSSStyleDeclaration.h>
#include <libweb/css/StyleProperties.h>
#include <libweb/Forward.h>

namespace Web::CSS {

struct MatchingRule {
    RefPtr<CSSStyleRule> rule;
    size_t style_sheet_index { 0 };
    size_t rule_index { 0 };
    size_t selector_index { 0 };
    u32 specificity { 0 };
};

class StyleResolver {
public:
    explicit StyleResolver(DOM::Document&);
    ~StyleResolver();

    DOM::Document& document() { return m_document; }
    DOM::Document const& document() const { return m_document; }

    NonnullRefPtr<StyleProperties> resolve_style(DOM::Element&) const;

    Vector<MatchingRule> collect_matching_rules(DOM::Element const&) const;
    void sort_matching_rules(Vector<MatchingRule>&) const;
    struct CustomPropertyResolutionTuple {
        Optional<StyleProperty> style {};
        u32 specificity { 0 };
    };
    CustomPropertyResolutionTuple resolve_custom_property_with_specificity(DOM::Element&, String const&) const;
    Optional<StyleProperty> resolve_custom_property(DOM::Element&, String const&) const;

    static bool is_inherited_property(CSS::PropertyID);

private:
    template<typename Callback>
    void for_each_stylesheet(Callback) const;

    DOM::Document& m_document;
};

}