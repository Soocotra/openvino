// Copyright (C) 2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <cstddef>

namespace ov {

/**
 * @brief Store optional object of type T (basic version of std::optional).
 *
 * @note If cpp17 used this class should be replaced by std::optional.
 *
 * @tparam T Type of stored object.
 */
template <class T>
class optional {
public:
    constexpr optional() noexcept = default;
    template <class... Args>
    constexpr optional(Args&&... args) : m_has_value{true},
                                         m_opt(std::forward<Args>(args)...) {}

    optional(const optional<T>& other) : m_has_value{other.m_has_value}, m_opt{} {
        if (other.m_has_value) {
            construct(*other);
        }
    }

    optional(optional<T>&& other) noexcept : m_has_value{other.m_has_value}, m_opt{} {
        if (other.m_has_value) {
            construct(std::move(*other));
        }
    }

    ~optional() {
        reset();
    }

    optional& operator=(const optional& other) {
        if (other.m_has_value) {
            if (m_has_value) {
                m_opt.m_value = *other;
            } else {
                construct(*other);
            }
            m_has_value = true;
        } else {
            reset();
        }
        return *this;
    }

    optional& operator=(optional&& other) noexcept {
        if (other.m_has_value) {
            if (m_has_value) {
                m_opt.m_value = std::move(*other);
            } else {
                construct(std::move(*other));
            }
            m_has_value = true;
        } else {
            reset();
        }
        return *this;
    }

    constexpr operator bool() const {
        return m_has_value;
    }

    constexpr const T& operator*() const& noexcept {
        return m_opt.m_value;
    }

    T& operator*() & noexcept {
        return m_opt.m_value;
    }

    constexpr const T&& operator*() const&& noexcept {
        return m_opt.m_value;
    }

    T&& operator*() && noexcept {
        return m_opt.m_value;
    }

    constexpr const T* operator->() const noexcept {
        return &m_opt.m_value;
    }

    T* operator->() noexcept {
        return &m_opt.m_value;
    }

    void reset() {
        if (m_has_value) {
            m_opt.m_value.T::~T();
            m_has_value = false;
        }
    }

private:
    template <class... Args>
    void construct(Args&&... args) {
        new (std::addressof(m_opt)) T(std::forward<Args>(args)...);
    }

    struct Empty {};

    template <class U>
    union Storage {
        Empty m_empty;
        T m_value;

        constexpr Storage() noexcept : m_empty{} {}
        constexpr Storage(uint8_t) noexcept : Storage{} {}

        template <class... Args>
        constexpr Storage(Args&&... args) : m_value(std::forward<Args>(args)...) {}
        ~Storage() {}
    };

    bool m_has_value;
    Storage<T> m_opt;
};
}  // namespace ov
