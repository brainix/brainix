/*----------------------------------------------------------------------------*\
 |	kernel.h							      |
 |									      |
 |	Copyright � 2002-2007, Team Brainix, original authors.		      |
 |		All rights reserved.					      |
\*----------------------------------------------------------------------------*/

/*
 | This program is free software: you can redistribute it and/or modify it under
 | the terms of the GNU General Public License as published by the Free Software
 | Foundation, either version 3 of the License, or (at your option) any later
 | version.
 |
 | This program is distributed in the hope that it will be useful, but WITHOUT
 | ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 | FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 | details.
 |
 | You should have received a copy of the GNU General Public License along with
 | this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <utime.h>

#include <config.h>
#include <const.h>

#include <kernel/multiboot.h>
#include <kernel/struct.h>
#include <kernel/message.h>
#include <kernel/prototype.h>
#include <kernel/syscall.h>

#include <fs/fs.h>
#include <driver/driver.h>

#endif
