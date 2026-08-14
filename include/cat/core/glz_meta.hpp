#pragma once

#include <glaze/glaze.hpp>
#include <cat/util/math.hpp>

template <>
struct glz::meta<glm::vec4> {
    using T = glm::vec4;
    static constexpr auto value = glz::object(
        "x", &T::x,
        "y", &T::y,
        "z", &T::z,
        "w", &T::w
    );
};

template <>
struct glz::meta<glm::vec3> {
    using T = glm::vec3;
    static constexpr auto value = glz::object(
        "x", &T::x,
        "y", &T::y,
        "z", &T::z
    );
};

template <>
struct glz::meta<glm::vec2> {
    using T = glm::vec2;
    static constexpr auto value = glz::object(
        "x", &T::x,
        "y", &T::y
    );
};

template <>
struct glz::meta<glm::ivec4> {
    using T = glm::ivec4;
    static constexpr auto value = glz::object(
        "x", &T::x,
        "y", &T::y,
        "z", &T::z,
        "w", &T::w
    );
};

template <>
struct glz::meta<glm::ivec3> {
    using T = glm::ivec3;
    static constexpr auto value = glz::object(
        "x", &T::x,
        "y", &T::y,
        "z", &T::z
    );
};

template <>
struct glz::meta<glm::ivec2> {
    using T = glm::ivec2;
    static constexpr auto value = glz::object(
        "x", &T::x,
        "y", &T::y
    );
};

template <>
struct glz::meta<glm::quat> {
    using T = glm::quat;
    static constexpr auto value = glz::object(
        "w", &T::w,
        "x", &T::x,
        "y", &T::y,
        "z", &T::z
    );
};

template <>
struct glz::from<glz::JSON, glm::mat4> {
    template <auto Opts>
    static void op(glm::mat4& value, is_context auto&& ctx, auto&& it, auto&& end) {
        std::array<float, 16> data{};
        parse<JSON>::op<Opts>(data, ctx, it, end);
        value = glm::make_mat4(data.data());
    }
};

template <>
struct glz::to<glz::JSON, glm::mat4> {
    template <auto Opts>
    static void op(const glm::mat4& value, is_context auto&& ctx, auto&& b, auto&& ix) noexcept {
        std::array<float, 16> data{};
        std::memcpy(data.data(), glm::value_ptr(value), sizeof(data));
        serialize<JSON>::op<Opts>(data, ctx, b, ix);
    }
};
