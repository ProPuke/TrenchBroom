/*
 Copyright (C) 2010-2013 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TrenchBroom__Brush__
#define __TrenchBroom__Brush__

#include "TrenchBroom.h"
#include "VecMath.h"
#include "Model/BrushTypes.h"
#include "Model/BrushFaceTypes.h"
#include "Model/Object.h"

#include <vector>

namespace TrenchBroom {
    namespace Model {
        class Brush : public Object {
        private:
            BrushFaceList m_faces;

            Brush(const BrushFaceList& faces);
        public:
            static BrushPtr newBrush(const BrushFaceList& faces);
            
            inline const BrushFaceList& faces() const {
                return m_faces;
            }
        };
    }
}

#endif /* defined(__TrenchBroom__Brush__) */