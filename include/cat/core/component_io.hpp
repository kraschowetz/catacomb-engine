#pragma once

#include "glaze/json/generic_fwd.hpp"   // IWYU pragma: export
#include <glaze/json.hpp>               // IWYU pragma: export

namespace cat
{

template<typename T>
concept GlazeReflectable = requires (T value, const glz::generic& json) {
    {glz::read_json<T>(json)};
};

template<typename T>
concept HasFromJson = requires (const glz::generic& json) {
    {T::from_json(json)};
};

template<typename T>
concept AutoLoadable = GlazeReflectable<T> && !HasFromJson<T>;

template<typename T>
concept LoadableComponent = HasFromJson<T> || AutoLoadable<T>;

template<typename T>
concept HasAsJson = requires (const T& val) {
    {T::as_json(val)};
};

template<typename T>
concept AutoWriteable = GlazeReflectable<T> && !HasAsJson<T>;

template<typename T>
concept WritableComponent = HasAsJson<T> || AutoWriteable<T>;

}
