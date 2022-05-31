#pragma once
#include <cstddef>
#include <memory>

class MinHook {
public:
    void init(void* base) noexcept;
    void restore() noexcept {}
    void hookAt(std::size_t index, void* fun) noexcept;

    template<typename T, std::size_t Idx, typename ...Args>
    constexpr auto getOriginal(Args... args) const noexcept
    {
        return reinterpret_cast<T(__thiscall*)(void*, Args...)>(originals[Idx]);
    }
    template<typename T, std::size_t Idx, typename ...Args>
    constexpr auto getOriginal__std(Args... args) const noexcept
    {
        return reinterpret_cast<T(__stdcall*)( Args...)>(originals[Idx]);
    }
    template<typename T, std::size_t Idx>
    constexpr auto get_manual_orig() const noexcept
    {
        return reinterpret_cast<T>(originals[Idx]);
    }
    bool _init = false;
private:

    void* base;
    std::unique_ptr<uintptr_t[]> originals;
};
