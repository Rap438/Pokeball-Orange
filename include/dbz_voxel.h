#ifndef GUARD_DBZ_VOXEL_H
#define GUARD_DBZ_VOXEL_H

#include "main.h"

// PokeBall Orange: 3D Flying Nimbus flight (src/dbz_voxel.c)
void DBZ_VoxelFlight_Prepare(u16 fromMapsec, u16 toMapsec);
bool8 DBZ_VoxelFlight_IsPending(void);
void DBZ_VoxelFlight_Start(MainCallback done);

extern u16 gDBZVoxelFrameRate;

#endif // GUARD_DBZ_VOXEL_H
