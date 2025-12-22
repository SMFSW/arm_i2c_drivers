#!/bin/sh

# before first commit, set the script as executable to git:
# git update-index --chmod=+x .cppcheck/cppcheck.sh

exec cppcheck \
  --enable=warning,style,performance \
  --inconclusive \
  --inline-suppr \
  --force \
  --check-level=exhaustive \
  --suppress=missingIncludeSystem \
  --suppressions-list=./.cppcheck/cppcheck.suppress \
  --error-exitcode=1 \
  --platform=arm32-wchar_t2 \
  '-D__cppcheck__' \
  '-D__GNUC__' \
  '-DUNUSED_RET="(void)"' \
  '-DNONNULLX__(...)=""' \
  '-DSECTION__(...)=""' \
  .