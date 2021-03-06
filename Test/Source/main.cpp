/*
 Copyright (C) 2010-2012 Kristian Duske
 
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

#include <iostream>

#include "TestSuite.h"
#include "Utility/FindIntegerPlanePointsTest.h"
#include "Utility/MatTest.h"
#include "Utility/PlaneTest.h"
#include "Utility/VecTest.h"

int main(int argc, const char * argv[]) {
    using namespace TrenchBroom;
    
    VecMath::VecTest vecTest;
    vecTest.run();
    
    VecMath::MatTest matTest;
    matTest.run();

    VecMath::PlaneTest planeTest;
    planeTest.run();
    
    /*
    VecMath::FindIntegerPlanePointsTest planePointsTest;
    planePointsTest.run();
    */
    
    return 0;
}

