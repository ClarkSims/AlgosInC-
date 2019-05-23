/// \file memory_access_specifier.h
/// \brief Enums that sepcify how inter process memory objects are created. These are used in the util_ipc library.

/***************************************************************************
 *   Copyright (C) 2014 by Clark Sims                                      *
 *   http://AcumenSoftwareInc.com/WhoWeAre/Clark_Sims.html                 *
 *   ClarkSims@AcumenSoftwareInc.com                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef memory_access_specifier_h
#define memory_access_specifier_h

namespace util_ipc {
    typedef enum {
      _create_only_  = 1,
      _open_or_create_,
      _open_only_,
      _use_new_,
      _read_only_,
      _read_write_
    } memory_access_specifier ;
};

#endif
