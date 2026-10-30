/*
 *
 *  Copyright (C) 2026, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module:  dcmiod
 *
 *  Purpose: Tests for setters of the ultrasound modules
 *
 */

#include "dcmtk/config/osconfig.h"

#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmiod/iodtypes.h"
#include "dcmtk/dcmiod/modenhusimage.h"
#include "dcmtk/dcmiod/modusfor.h"
#include "dcmtk/ofstd/oftest.h"
#include "dcmtk/ofstd/ofvector.h"

static OFVector<Float64> makeMatrix(const Float64 start)
{
    OFVector<Float64> m;
    for (size_t n = 0; n < 16; n++)
        m.push_back(start + OFstatic_cast(Float64, n));
    return m;
}

// setImageType() always builds four values, so the value check must use VM 4
// (Image Type in the Enhanced US Image Module, PS3.3 C.8.24.3.2).
OFTEST(dcmiod_enhus_set_image_type)
{
    if (!dcmDataDict.isDictionaryLoaded())
    {
        OFCHECK_FAIL("no data dictionary loaded, set " DCM_DICT_ENVIRONMENT_VARIABLE);
        return;
    }

    IODEnhUSImageModule mod;
    OFString value;

    OFCHECK(mod.setImageType(DcmIODTypes::IMAGETYPE_ORIGINAL, "TISSUE", "NONE").good());
    OFCHECK(mod.getImageType(value, -1).good());
    OFCHECK_EQUAL(value, "ORIGINAL\\PRIMARY\\TISSUE\\NONE");
}

// Calling setApexPosition() a second time must replace the first value.
OFTEST(dcmiod_usfor_set_apex_position_twice)
{
    if (!dcmDataDict.isDictionaryLoaded())
    {
        OFCHECK_FAIL("no data dictionary loaded, set " DCM_DICT_ENVIRONMENT_VARIABLE);
        return;
    }

    IODUSFoRModule mod;
    OFVector<Float64> value;

    OFCHECK(mod.setApexPosition(1.0, 2.0, 3.0).good());
    OFCHECK(mod.setApexPosition(4.0, 5.0, 6.0).good());
    OFCHECK(mod.getApexPosition(value).good());
    OFCHECK_EQUAL(value.size(), 3);
    if (value.size() == 3)
    {
        OFCHECK_EQUAL(value[0], 4.0);
        OFCHECK_EQUAL(value[1], 5.0);
        OFCHECK_EQUAL(value[2], 6.0);
    }
}

// Calling the mapping matrix setters a second time must replace the first value.
OFTEST(dcmiod_usfor_set_mapping_matrices_twice)
{
    if (!dcmDataDict.isDictionaryLoaded())
    {
        OFCHECK_FAIL("no data dictionary loaded, set " DCM_DICT_ENVIRONMENT_VARIABLE);
        return;
    }

    IODUSFoRModule mod;
    Float64 value = 0;

    OFCHECK(mod.setVolumetoTransducerMappingMatrix(makeMatrix(1.0)).good());
    OFCHECK(mod.setVolumetoTransducerMappingMatrix(makeMatrix(100.0)).good());
    OFCHECK(mod.getVolumetoTransducerMappingMatrix(value, 0).good());
    OFCHECK_EQUAL(value, 100.0);

    OFCHECK(mod.setVolumeToTableMappingMatrix(makeMatrix(1.0)).good());
    OFCHECK(mod.setVolumeToTableMappingMatrix(makeMatrix(200.0)).good());
    OFCHECK(mod.getVolumetoTableMappingMatrix(value, 0).good());
    OFCHECK_EQUAL(value, 200.0);
}

// Calling setDepthsOfFocus() with a vector a second time must replace the first value.
OFTEST(dcmiod_enhus_set_depths_of_focus_twice)
{
    if (!dcmDataDict.isDictionaryLoaded())
    {
        OFCHECK_FAIL("no data dictionary loaded, set " DCM_DICT_ENVIRONMENT_VARIABLE);
        return;
    }

    IODEnhUSImageModule mod;
    OFVector<Float64> first;
    first.push_back(10.0);
    first.push_back(20.0);
    OFVector<Float64> second;
    second.push_back(30.0);
    second.push_back(40.0);
    Float64 value = 0;

    OFCHECK(mod.setDepthsOfFocus(first).good());
    OFCHECK(mod.setDepthsOfFocus(second).good());
    OFCHECK(mod.getDepthsOfFocus(value, 0).good());
    OFCHECK_EQUAL(value, 30.0);
    OFCHECK(mod.getDepthsOfFocus(value, 1).good());
    OFCHECK_EQUAL(value, 40.0);
}
