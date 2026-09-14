#!/bin/sh

echo "== $0 $@"

file="$1"
if [ -z "$file" ]; then echo "file is empty"; exit 0; fi

# disable cxxtools log_trace() calls due to
# incompatibility with our fty-common-logging macros
sed -i -E '/^[ \t]*log_trace\(\"/ s/^#*/\/\/CXX_LOGTRACE_DISABLED/' "$file"

# make [[maybe_unused]] arguments in _component_ constructor to avoid compilation warnings
sed -i 's|(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)|([[maybe_unused]] tnt::HttpRequest\& request, [[maybe_unused]] tnt::HttpReply\& reply, [[maybe_unused]] tnt::QueryParams\& qparam)|g' "$file"

##cat "$file" | grep "log_trace"

exit 0
