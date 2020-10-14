#pragma once
#include <pack/pack.h>
#include <regex>

namespace fty::rest {

// =====================================================================================================================

/// Translate message structure
struct TranslateMessage : public pack::Node
{
    pack::String    key  = FIELD("key");
    pack::StringMap vars = FIELD("variables");

    using pack::Node::Node;
    META(TranslateMessage, key, vars);
};

// =====================================================================================================================

struct ITranslate
{
    virtual ~ITranslate()                = default;
    virtual std::string toString() const = 0;
    virtual std::string toJson() const   = 0;
};

// =====================================================================================================================

template <typename... Args>
struct Translate : public ITranslate
{
    Translate(const std::string& msg, const Args&... args)
        : m_msg(msg)
        , m_args{args...}
    {
    }

    Translate(const Translate&) = delete;
    Translate(Translate&&)      = default;
    Translate& operator=(const Translate&) = delete;
    Translate& operator=(Translate&&) = default;

    operator std::string() const
    {
        return toString();
    }

    std::string toString() const override
    {
        return std::apply(
            [&](const auto&... args) {
                return fmt::format(m_msg, args...);
            },
            m_args);
    }

    std::string toJson() const override
    {
        TranslateMessage tr;

        static std::regex rex("\\{([^\\}]*)\\}");
        static std::regex rex2("@\\[([^(\\]@)]*)\\]@");

        std::smatch match;
        std::string view(m_msg);
        std::string newmsg;
        while (std::regex_search(view, match, rex)) {
            newmsg += match.prefix().str() + "@[{" + match[1].str() + "}]@";
            view = match.suffix();
        }
        newmsg += view;

        newmsg = std::apply(
            [&](const auto&... args) {
                return fmt::format(newmsg, args...);
            },
            m_args);

        std::string res;
        view      = newmsg;
        int index = 0;
        while (std::regex_search(view, match, rex2)) {
            std::string name = "var" + std::to_string(++index);
            res += match.prefix().str() + "{{" + name + "}}";
            tr.vars.append(name, match[1].str());
            view = match.suffix();
        }
        res += view;
        tr.key = res;

        return *pack::json::serialize(tr);
    }

private:
    std::string         m_msg;
    std::tuple<Args...> m_args;
};

// =====================================================================================================================
// Translate deduction helper.
// =====================================================================================================================

namespace details {
    template <typename T>
    struct Types
    {
        using type = std::decay_t<T>;
    };

    template <int N>
    struct Types<char[N]>
    {
        using type = std::string;
    };

    template <typename T>
    using UseType = typename Types<T>::type;
} // namespace details

template <typename... Args>
Translate(const std::string&, const Args&...) -> Translate<details::UseType<Args>...>;

// =====================================================================================================================

} // namespace fty::rest
