/**
 * Thrift files can namespace, package, or prefix their output in various
 * target languages.
 */
namespace py firtex
namespace cpp firtex
namespace java firtex
namespace php firtex
namespace perl firtex

enum WatchdogErrorCode {
    WATCHDOG_EC_OK = 0,
    WATCHDOG_EC_PROCESS_EXIST = 1,
    WATCHDOG_EC_PROCESS_NOT_FOUND = 2,
    WATCHDOG_EC_NO_PERMISSION = 3,
    WATCHDOG_EC_SERIALIZE_FAIL = 4,
    WATCHDOG_EC_SYSTEM_ERROR = 5,
}

struct WatchdogErrorInfo {
    1: WatchdogErrorCode errorCode,
    2: string errorMsg
}

struct WatchdogCommand {
    1: string processName,
    2: list<string> parameters
}

struct WatchdogExecutionTask {
    1: WatchdogCommand command,
    2: map<string, string> envVariables,
    3: i64 processInfoKeepTime,
    4: string workDirectory,
    5: bool restartAfterCrash
}

struct WatchdogExecutionResult {
    1: WatchdogErrorInfo errorInfo,
    2: i32 pid
}

struct WatchdogProcessStatus {
    1: WatchdogErrorInfo errorInfo
    2: i32 pid,
    3: i32 status,
    4: i32 restartCount,
    5: i32 lastCrashTime,
    6: WatchdogCommand command,
    7: i32 stopSignal
}

/**
 * Define a daemon service. 
 */
service ThriftWatchdogService {
    WatchdogExecutionResult startProcess(1:WatchdogExecutionTask task),
    WatchdogErrorInfo stopProcess(1:WatchdogCommand cmd, 2: i32 stopSignal),
    WatchdogProcessStatus getProcessStatus(1:WatchdogCommand cmd),
    void clear(),
    bool ping();
}
