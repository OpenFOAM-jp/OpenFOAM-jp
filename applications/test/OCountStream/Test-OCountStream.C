/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017-2018 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description

\*---------------------------------------------------------------------------*/

#include "OCountStream.H"
#include "StringStream.H"
#include "IOstreams.H"
#include "argList.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    OCountStream cnt;
    OStringStream str;

    ocountstream plain;

    for (label i = 0; i < 50; ++i)
    {
        str
            << 1002 << " " << "abcd" << " "
            << "def" << " " << 3.14159 << ";\n";

        cnt
            << 1002 << " " << "abcd" << " "
            << "def" << " " << 3.14159 << ";\n";

        plain
            << 1002 << " " << "abcd" << " "
            << "def" << " " << 3.14159 << ";\n";
    }

    cnt.print(Info);
    Info<< "via string-stream: " << str.str().size() << " chars" << endl;
    Info<< "via ocountstream: " << plain.size() << " chars" << endl;

    Info<< "\nEnd\n" << endl;

    return 0;
}


// ************************************************************************* //
