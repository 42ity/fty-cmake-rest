/*  =========================================================================
    Copyright (C) 2018 - 2020 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

#pragma once

#include <fty_log.h>
#include <fty/translate.h>


// =====================================================================================================================

#define auditError(...)\
    auditLog(log4cplus::ERROR_LOG_LEVEL, __VA_ARGS__)

#define auditDebug(...)\
    auditLog(log4cplus::DEBUG_LOG_LEVEL, __VA_ARGS__)

#define auditInfo(...)\
    auditLog(log4cplus::INFO_LOG_LEVEL, __VA_ARGS__)

#define auditWarn(...)\
    auditLog(log4cplus::WARN_LOG_LEVEL, __VA_ARGS__)

#define auditFatal(...)\
    auditLog(log4cplus::FATAL_LOG_LEVEL, __VA_ARGS__)

#define auditLog(level, ...) \
    fty::audit::LogManager::getInstance()->insertLog(level, __FILE__, __LINE__, __func__, fty::audit::format(__VA_ARGS__).c_str())

// =====================================================================================================================

namespace fty::audit {

inline Ftylog AUDITLOG("www-audit", "/etc/fty/wwwlog.cfg");

// singleton for logger management
class LogManager
{

public:
    // Return singleton Audit Ftylog instance
    static Ftylog* getInstance()
    {
        return &AUDITLOG;
    }

    /// Set audit log context.
    /// @param token The token
    /// @param username The user name
    /// @param userId The user id
    static void setAuditLogContext(
        const std::string& token, const std::string& username, const int userId, const std::string& ip)
    {
        Ftylog::clearContext();

        // clang-format off
        Ftylog::setContext({
            {"sessionId", std::to_string(std::hash<std::string>{}(token))},
            {"username" , username},
            {"uid"      , std::to_string(userId)},
            {"ip"       , ip}
        });
        // clang-format on
    }

    /// Clear audit log context.
    static void clearAuditLogContext()
    {
        Ftylog::clearContext();
    }
};

inline std::string format(const std::string& str)
{
    return str;
}

template<typename... Args>
inline std::string format(const std::string& str, const Args&... args)
{
    return fmt::format(str, args...);
}

template<typename... Args>
inline std::string format(Translate& trans, const Args&... args)
{
    return trans.format(args...).toString();
}

} // namespace fty::audit
