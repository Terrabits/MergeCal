#-------------------------------------------------
# 
# Project  : MergeCal
# App name : R&S Merge Cal
# 
# 
# RsaToolbox
# Template : Rsa Vna Macro Without Ui
#            Version 2.0.0
# 
# (C) Rohde & Schwarz America
# 
#-------------------------------------------------



TARGET       = MergeCal
TEMPLATE     = app

include(core.pri)
SOURCES     += main.cpp
OTHER_FILES += DEBUG_LOGFILE.txt

# Executable Icon
win32: RC_FILE = VS2010Resources.rc
