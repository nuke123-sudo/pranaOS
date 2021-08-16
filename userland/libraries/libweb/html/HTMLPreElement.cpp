/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLPreElement.h>

namespace Web::HTML {

HTMLPreElement::HTMLPreElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLPreElement::~HTMLPreElement()
{
}

}