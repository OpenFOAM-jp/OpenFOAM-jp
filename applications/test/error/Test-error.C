/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "error.H"
#include "IOstreams.H"
#include "dictionary.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    FatalError.throwExceptions();

    try
    {
        WarningInFunction << "warning 1" << endl;
        IOWarningInFunction(Serr) << "warning 2" << endl;

        dictionary dict;

        IOWarningInFunction(dict) << "warning 3" << endl;

        FatalErrorInFunction
            << "This is an error from 1" << nl
            << "Explanation to follow:" << endl;

        FatalErrorInFunction
            << "Error 2"
            << exit(FatalError);
    }
    catch (const Foam::error& fErr)
    {
        Serr<< "Caught Foam error " << fErr << nl << endl;
    }

    try
    {
        FatalErrorInFunction
            << "Error# 3"
            << exit(FatalError);
    }
    catch (const Foam::error& fErr)
    {
        Serr<< "Caught Foam error " << fErr << nl << endl;
    }

    return 0;
}


// ************************************************************************* //
