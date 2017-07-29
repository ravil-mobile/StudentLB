#include <unordered_map>

#include "DataStructure.h"

#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

/** handles the boundaries in our simulation setup */

void scanBoundary( std::list<BoundaryFluid*>& ObstacleList,
					std::vector<Fluid*>& FluidDomain,
					std::vector<Fluid*>& VTKrepresentation,
                    int* flagField,
					int *VtkID,
					std::vector<BoundaryEntry*> BoundaryConditions,
 					int *CpuID,
 					int RANK,
 					std::unordered_map<unsigned, unsigned>& LocalToGlobalIdTable,
 					std::vector<BoundaryBuffer>& CommunicationBuffers );


void treatBoundary( double *collideField,
                    std::list<BoundaryFluid*>& BoundaryLayerList );

#endif
