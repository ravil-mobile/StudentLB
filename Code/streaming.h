#include <vector>
#include "DataStructure.h"

#ifndef _STREAMING_H_
#define _STREAMING_H_

/** carries out the streaming step and writes the respective distribution functions from
 *  collideField to streamField.
 */
void doStreaming( double *collideField,
                  double *streamField,
                  std::vector<Fluid*>& FluidDomain );

#endif
