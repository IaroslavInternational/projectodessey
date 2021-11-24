#pragma once

// Нацеливание на Windows 7 и выше
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>
// Данные #define отключают много неиспользуемого. Если выходят
// ошибки из-за этого - включить файл
// или сразу все, что увеличит время загрузки.

#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

#define NOMINMAX
#define STRICT

#include <Windows.h>

#include "EngineConverter.h"