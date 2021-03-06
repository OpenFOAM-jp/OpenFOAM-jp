/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "Curle.H"
#include "fvcDdt.H"
#include "mathematicalConstants.H"
#include "addToRunTimeSelectionTable.H"

using namespace Foam::constant;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(Curle, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        Curle,
        dictionary
    );
}
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

bool Foam::functionObjects::Curle::calc()
{
    if (foundObject<volScalarField>(fieldName_))
    {
        // Evaluate pressure force time derivative

        const volScalarField& p = lookupObject<volScalarField>(fieldName_);
        const volScalarField dpdt(scopedName("dpdt"), fvc::ddt(p));
        const volScalarField::Boundary& dpdtBf = dpdt.boundaryField();
        const surfaceVectorField::Boundary& SfBf = mesh_.Sf().boundaryField();

        dimensionedVector dfdt("dfdt", p.dimensions()*dimArea/dimTime, Zero);

        for (auto patchi : patchSet_)
        {
            dfdt.value() += sum(dpdtBf[patchi]*SfBf[patchi]);
        }

        reduce(dfdt.value(), sumOp<vector>());


        // Construct and store Curle acoustic pressure

        const volVectorField& C = mesh_.C();

        auto tpDash = tmp<volScalarField>::New
        (
            IOobject
            (
                resultName_,
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar(p.dimensions(), Zero)
        );
        auto& pDash = tpDash.ref();

        const volVectorField d(scopedName("d"), C - x0_);
        pDash = (d/magSqr(d) & dfdt)/(4.0*mathematical::pi*c0_);

        return store(resultName_, tpDash);
    }

    return false;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::Curle::Curle
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fieldExpression(name, runTime, dict, "p"),
    patchSet_(),
    x0_("x0", dimLength,  Zero),
    c0_("c0", dimVelocity, Zero)
{
    read(dict);

    setResultName(typeName, fieldName_);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::Curle::~Curle()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::Curle::read(const dictionary& dict)
{
    if (fieldExpression::read(dict))
    {
        patchSet_ = mesh_.boundaryMesh().patchSet(dict.get<wordRes>("patches"));

        if (patchSet_.empty())
        {
            WarningInFunction
                << "No patches defined"
                << endl;

            return false;
        }

        // Read the reference speed of sound
        dict.readEntry("c0", c0_);


        // Set the location of the effective point source to the area-average
        // of the patch face centres
        const volVectorField::Boundary& Cbf = mesh_.C().boundaryField();
        const surfaceScalarField::Boundary& magSfBf =
            mesh_.magSf().boundaryField();

        x0_.value() = vector::zero;
        scalar sumMagSf = 0;
        for (auto patchi : patchSet_)
        {
            x0_.value() += sum(Cbf[patchi]*magSfBf[patchi]);
            sumMagSf += sum(magSfBf[patchi]);
        }

        reduce(x0_.value(), sumOp<vector>());
        reduce(sumMagSf, sumOp<scalar>());

        x0_.value() /= sumMagSf + ROOTVSMALL;

        return true;
    }

    return false;
}


// ************************************************************************* //
