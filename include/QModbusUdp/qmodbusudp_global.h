// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QtCore/QtGlobal>

#if defined(QMODBUSUDP_LIBRARY)
#  define QMBUDP_EXPORT Q_DECL_EXPORT
#else
#  define QMBUDP_EXPORT Q_DECL_IMPORT
#endif
