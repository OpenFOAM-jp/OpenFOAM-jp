/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2016 OpenCFD Ltd.
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

Class
    Foam::functionObjects::surfaceInterpolate

Group
    grpFieldFunctionObjects

Description
    Linearly interpolates volume fields to generate surface fields.

    Fields are stored
    - every time step the field is updated with new values
    - at output it writes the fields

    This functionObject can either be used
    - to calculate a new field as a  post-processing step or
    - since the fields are registered, used in another functionObject

Usage
    Example of function object specification:
    \verbatim
    surfaceInterpolate1
    {
        type        surfaceInterpolate;
        libs        ("libfieldFunctionObjects.so");
        ...
        fields      ((p pNear)(U UNear));
    }
    \endverbatim

    Where the entries comprise:
    \table
        Property | Description               | Required    | Default value
        type     | type name: nearWallFields | yes         |
        fields   | list of fields with correspoding output field names | yes |
        log      | Log to standard output    | no          | yes
    \endtable


See also
    Foam::functionObjects::fvMeshFunctionObject
    Foam::functionObjects::timeControl

SourceFiles
    surfaceInterpolate.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_surfaceInterpolate_H
#define functionObjects_surfaceInterpolate_H

#include "fvMeshFunctionObject.H"
#include "surfaceFieldsFwd.H"
#include "Tuple2.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                  Class surfaceInterpolate Declaration
\*---------------------------------------------------------------------------*/

class surfaceInterpolate
:
    public fvMeshFunctionObject
{
protected:

    // Protected data

        //- Fields to process
        List<Tuple2<word, word>> fieldSet_;

        //- Switch to send output to Info as well as to file
        Switch log_;


    // Protected Member Functions

        template<class Type>
        void interpolateFields();


private:

    // Private member functions

        //- No copy construct
        surfaceInterpolate(const surfaceInterpolate&) = delete;

        //- No copy assignment
        void operator=(const surfaceInterpolate&) = delete;


public:

    //- Runtime type information
    TypeName("surfaceInterpolate");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        surfaceInterpolate
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~surfaceInterpolate();


    // Member Functions

        //- Read the controls
        virtual bool read(const dictionary&);

        //- Calculate the interpolated fields
        virtual bool execute();

        //- Write the interpolated fields
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "surfaceInterpolateTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
