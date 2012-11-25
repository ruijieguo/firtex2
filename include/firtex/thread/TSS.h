/**
 * Copyright (C) 2007 Guo Ruijie. All rights reserved.
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 *
 */

#ifndef __FXTSSSELECT_H__
#define __FXTSSSELECT_H__

#ifdef HAVE_CONFIG_H
#include "firtex/config.h"
#endif

// Select the correct TSS implementation based on
// what the compilation environment has defined

#if defined(FX_POSIX)

#include "TSS_POSIX.h"

#elif defined(FX_WINDOWS)

#include "TSS_WIN32.h"

#elif defined(FX_MACOS)

#include "TSS_MACOS.h"

#endif


#ifndef __FXTSS_H__
#error "No TSS implementation could be selected"
#endif

#endif // __FXTSSSELECT_H__
