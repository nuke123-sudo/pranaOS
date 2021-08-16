/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/FlyString.h>
#include <base/StringBuilder.h>
#include <libweb/bindings/LocationObject.h>
#include <libweb/bindings/WindowObject.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Window.h>

namespace Web {
namespace Bindings {

LocationObject::LocationObject(JS::GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void LocationObject::initialize(JS::GlobalObject& global_object)
{
    Object::initialize(global_object);
    u8 attr = JS::Attribute::Writable | JS::Attribute::Enumerable;
    define_native_accessor("href", href_getter, href_setter, attr);
    define_native_accessor("host", host_getter, {}, attr);
    define_native_accessor("hostname", hostname_getter, {}, attr);
    define_native_accessor("pathname", pathname_getter, {}, attr);
    define_native_accessor("hash", hash_getter, {}, attr);
    define_native_accessor("search", search_getter, {}, attr);
    define_native_accessor("protocol", protocol_getter, {}, attr);

    define_native_function("reload", reload, 0, JS::Attribute::Enumerable);
}

LocationObject::~LocationObject()
{
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::href_getter)
{
    auto& window = static_cast<WindowObject&>(global_object);
    return JS::js_string(vm, window.impl().document().url().to_string());
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::href_setter)
{
    auto& window = static_cast<WindowObject&>(global_object);
    auto new_href = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    auto href_url = window.impl().document().complete_url(new_href);
    if (!href_url.is_valid()) {
        vm.throw_exception<JS::URIError>(global_object, String::formatted("Invalid URL '{}'", new_href));
        return {};
    }
    window.impl().did_set_location_href({}, href_url);
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::pathname_getter)
{
    auto& window = static_cast<WindowObject&>(global_object);
    return JS::js_string(vm, window.impl().document().url().path());
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::hostname_getter)
{
    auto& window = static_cast<WindowObject&>(global_object);
    return JS::js_string(vm, window.impl().document().url().host());
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::host_getter)
{
    auto& window = static_cast<WindowObject&>(global_object);
    auto url = window.impl().document().url();
    return JS::js_string(vm, String::formatted("{}:{}", url.host(), url.port()));
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::hash_getter)
{
    auto& window = static_cast<WindowObject&>(global_object);
    auto fragment = window.impl().document().url().fragment();
    if (!fragment.length())
        return JS::js_string(vm, "");
    StringBuilder builder;
    builder.append('#');
    builder.append(fragment);
    return JS::js_string(vm, builder.to_string());
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::search_getter)
{
    auto& window = static_cast<WindowObject&>(global_object);
    auto query = window.impl().document().url().query();
    if (!query.length())
        return JS::js_string(vm, "");
    StringBuilder builder;
    builder.append('?');
    builder.append(query);
    return JS::js_string(vm, builder.to_string());
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::protocol_getter)
{
    auto& window = static_cast<WindowObject&>(global_object);
    StringBuilder builder;
    builder.append(window.impl().document().url().protocol());
    builder.append(':');
    return JS::js_string(vm, builder.to_string());
}

JS_DEFINE_NATIVE_FUNCTION(LocationObject::reload)
{
    auto& window = static_cast<WindowObject&>(global_object);
    window.impl().did_call_location_reload({});
    return JS::js_undefined();
}

}

}