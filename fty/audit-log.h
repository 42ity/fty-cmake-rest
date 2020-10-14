#pragma once

#include <fmt/core.h>
#include <fty_log.h>


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
    fty::audit::LogManager::getInstance()->insertLog(level, __FILE__, __LINE__, __func__, fmt::format(__VA_ARGS__).c_str())

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

} // namespace fty::audit
