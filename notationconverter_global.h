/*
* Copyright (c) 2017 Vitali Parkhomenko
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#pragma once

#include <QtGlobal>

#if defined(NOTATIONCONVERTER_LIBRARY)
#  define NOTATIONCONVERTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NOTATIONCONVERTERSHARED_EXPORT Q_DECL_IMPORT
#endif
