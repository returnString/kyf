#pragma once

class Log;
class LogSegment;

using WritableLogSegment = std::shared_ptr<LogSegment>;
using ReadOnlyLogSegment = std::shared_ptr<const LogSegment>;