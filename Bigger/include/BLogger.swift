//
//  BLogger.swift
//  Pods
//
//  Created by 王仕杰 on 2017/9/12.
//
//

private let dateFormatter: DateFormatter = {
    let formatter = DateFormatter()
    formatter.dateFormat = "yyyy-MM-dd HH:mm:ss.SSS"
    return formatter
}()

public enum BiggerLogLevel: String {
    case info = "[I]"
    case debug = "[D]"
    case error = "[E]"
    case fatal = "[F]"
}

private func logPrefix(level: BiggerLogLevel, codeInfo: (file: String, function: String, line: Int)) -> String {

    return  "[\(dateFormatter.string(from: Date()))]"
            + "[\(getpid()):\(String(format: "%02d", Thread.current.value(forKeyPath: "private.seqNum") as! Int))" + (Thread.current.isMainThread ? "*]" : "]")
            + level.rawValue
            + "[\(Bundle.main.infoDictionary?["CFBundleName"] ?? "undefine")]"
            + "[\(codeInfo.0.components(separatedBy: "/").last!), \(codeInfo.1), \(String(format: "%03d", codeInfo.2))] "
}

public func log(_ msg: String, level: BiggerLogLevel = .info, file: String = #file, function: String = #function, line: Int = #line) {
    
    let prefix = logPrefix(level: level, codeInfo: (file, function, line))
    
    switch level {
    case .info:
        SwiftBiggerLog(B_LOG_TYPE_INFO, prefix + msg)
    case .debug:
        SwiftBiggerLog(B_LOG_TYPE_DEBUG, prefix + msg)
    case .error:
        SwiftBiggerLog(B_LOG_TYPE_ERROR, prefix + msg)
    case .fatal:
        SwiftBiggerLog(B_LOG_TYPE_FATAL, prefix + msg)
    }
    
}
