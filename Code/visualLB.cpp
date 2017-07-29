#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>

#include "helper.h"
#include "visualLB.h"
#include "computeCellValues.h"
#include "LBDefinitions.h"
#include "DataStructure.h"



//Writes Density and Velocity from the collision field
void writeVtkOutput( const char * filename,
                     int RANK,
                     double* const collideField,
                     int* VtkID,
                     std::vector<Fluid*>& FluidDomain,
                     std::vector<Fluid*>& VTKrepresentation,
                     unsigned int TimeStep ) {


    char szFilename[ 80 ];
    FILE *fp = NULL;
    sprintf( szFilename, "%s%i.%i.vtk", filename, RANK ,TimeStep );


    fp = fopen( szFilename, "w");
    if( fp == NULL ) {
        char szBuff[ 80 ];
        sprintf( szBuff, "Failed to open %s", filename );
        ERROR( szBuff );
        return;
    }


    write_vtkHeader( fp, FluidDomain );
    write_vtkPointCoordinates( fp, FluidDomain );
    write_vtkPointElements( fp, VTKrepresentation, VtkID );


    fprintf( fp, "\nPOINT_DATA %lu \n", FluidDomain.size() );
//------------------------------------------------------------------------------
//                        Write density to the file
//------------------------------------------------------------------------------
    //Computing Density
    fprintf( fp, "SCALARS density float 1 \n" );
    fprintf( fp, "LOOKUP_TABLE default \n" );

    // DEBUGGING: chech computeDensity
#ifdef DEBUGGING
    const double MAXIMUM_DENSITY = 1.1;
    const double MINIMUM_DENSITY = 0.9;
#endif


    double* Index = 0;
    double Density = 0.0;

	for ( std::vector<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell ) {

                Index = collideField + ( Vel_DOF * ((*aFluidCell)->getIndex( SELF_INDEX )) );
                computeDensity ( Index, &Density );

                fprintf( fp, "%f\n", Density );

#ifdef DEBUGGING
        if( ( Density < MINIMUM_DENSITY ) || ( Density > MAXIMUM_DENSITY ) )
        printf("x %f y %f z %f  %f   \n", (*aFluidCell)->getXCoord(),
                                          (*aFluidCell)->getYCoord(),
                                          (*aFluidCell)->getZCoord(),
                                          Density);
#endif

    }


//------------------------------------------------------------------------------
//                        Write velocity magnitude to the file
//------------------------------------------------------------------------------
    //Computing Velocity
    fprintf( fp, "\nSCALARS VelocityMagnitude float 1 \n" );
    fprintf( fp, "LOOKUP_TABLE default \n" );
    
    double Velocity[ 3 ] = { 0.0 };
    double VelocityMagnitude = 0.0;
    Index = 0;

    for ( std::vector<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell ) {

                Index = collideField + ( Vel_DOF * ((*aFluidCell)->getIndex( SELF_INDEX )) );

                computeDensity ( Index, &Density );
                computeVelocity ( Index, &Density, Velocity );

                VelocityMagnitude = Velocity [ 0 ] * Velocity [ 0 ]
                                  + Velocity [ 1 ] * Velocity [ 1 ]
                                  + Velocity [ 2 ] * Velocity [ 2 ];

                VelocityMagnitude = sqrt( VelocityMagnitude );
                fprintf( fp, "%f\n", VelocityMagnitude );
    }


//------------------------------------------------------------------------------
//                            Write CPU ID
//------------------------------------------------------------------------------
    fprintf( fp, "\nSCALARS CpuID float 1 \n" );
    fprintf( fp, "LOOKUP_TABLE default \n" );

	for ( std::vector<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell ) {

            fprintf( fp, "%d\n", (*aFluidCell)->getCpuID() );

    }	



//------------------------------------------------------------------------------
//                        Write velocity to the file
//------------------------------------------------------------------------------
    //Computing Velocity
    fprintf( fp, "\nVECTORS velocity float \n" );
    Index = 0;

    for ( std::vector<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell ) {

				Index = collideField + ( Vel_DOF * ((*aFluidCell)->getIndex( SELF_INDEX )) );

                computeDensity ( Index, &Density );
                computeVelocity ( Index, &Density, Velocity );


                fprintf( fp, "%f %f %f\n", Velocity [ 0 ],
                                           Velocity [ 1 ],
                                           Velocity [ 2 ] );
    }


    fclose( fp );
}


void write_vtkHeader( FILE *fp,
                      std::vector<Fluid*>& FluidDomain ) {

    if( fp == NULL ) {
        char szBuff[80];
        sprintf( szBuff, "Null pointer in write_vtkHeader" );
        ERROR( szBuff );
        return;
    }

    fprintf(fp,"# vtk DataFile Version 2.0\n");
    fprintf(fp,"generated by CFD-lab course output \n");
    fprintf(fp,"ASCII\n");
    fprintf(fp,"\n");
    fprintf(fp,"DATASET UNSTRUCTURED_GRID\n");
    fprintf(fp,"POINTS %d float\n", (int)FluidDomain.size() );
    fprintf(fp,"\n");

}


void write_vtkPointCoordinates( FILE* fp,
                                std::vector<Fluid*>& FluidDomain ) {

    for ( unsigned i = 0; i < FluidDomain.size(); ++i ) {

              fprintf(fp, "%f %f %f\n", FluidDomain[ i ]->getXCoord(),
                                        FluidDomain[ i ]->getYCoord(),
                                        FluidDomain[ i ]->getZCoord() );
    }
}


void write_vtkPointElements( FILE* fp,
                             std::vector<Fluid*>& VtkElements,
                             int* VtkID ) {


    // see the documentation: http://www.vtk.org/data-model/
    const char CODE_OF_ELEMENT[] = "11";
    const unsigned NUMBER_OF_POINT_PER_ELEMENT = 8;
    unsigned nElements = VtkElements.size();
    unsigned nEntries = ( NUMBER_OF_POINT_PER_ELEMENT + 1 ) * ( nElements );


    std::string DELIMITER = " ";
    std::string Line = "";

    // Print all elements
    fprintf(fp,"\nCELLS %u %u\n", nElements, nEntries );

    for ( unsigned i = 0; i < VtkElements.size(); ++i ) {

              Line.clear();

              // Print number entries that a line contains
              Line += std::to_string( NUMBER_OF_POINT_PER_ELEMENT );
              Line += DELIMITER;

              // walk around in the cell's neighbor according to VTL_VOXEL = 11
              // see the documentation: http://www.vtk.org/data-model/
              // Vertex 0:
              Line += std::to_string( VtkID [ VtkElements[ i ]->getIdIndex( 9 ) ] );
              Line += DELIMITER;

              Line += std::to_string( VtkID [ VtkElements[ i ]->getIdIndex( 10 ) ] );
              Line += DELIMITER;


              Line += std::to_string( VtkID [ VtkElements[ i ]->getIdIndex( 12 ) ] );
              Line += DELIMITER;


              Line += std::to_string( VtkID [ VtkElements[ i ]->getIdIndex( 13 ) ] );
              Line += DELIMITER;

              Line += std::to_string( VtkID [ VtkElements[ i ]->getIdIndex( 16 ) ] );
              Line += DELIMITER;

              Line += std::to_string( VtkID [ VtkElements[ i ]->getIdIndex( 17 ) ] );
              Line += DELIMITER;

              Line += std::to_string( VtkID [ VtkElements[ i ]->getIdIndex( 18 ) ] );
              Line += DELIMITER;

              Line += std::to_string( VtkID [ VtkElements[ i ]->getDiagonalLattice() ] );
              Line += DELIMITER;

             fprintf(fp, "%s\n", Line.c_str());

    }

    // Print type of the elements
    fprintf(fp,"\nCELL_TYPES %u\n", nElements );
    for ( unsigned i = 0; i < VtkElements.size(); ++i ) {
        fprintf(fp, "%s\n", CODE_OF_ELEMENT );
    }

}
